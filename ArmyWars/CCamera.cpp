#include "pch.h"
#include "CCamera.h"

#include "CEngine.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CUIMgr.h"
#include "CLogMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_CamSpeed(600.f)
	, m_BlackFilterTex(nullptr)
	, m_Frequency(0.f)
	, m_Amplitude(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
	, m_CamShake(false)
	, m_FilterInfo{}
	, m_DoorHoldDuration(0.f)
{
	// Black Filter Texture 생성
	Vec2 vResolution = CEngine::Get()->GetResolution();
	m_BlackFilterTex = CAssetMgr::Get()->CreateTexture(L"BlackFliterTex", (UINT)vResolution.x, (UINT)vResolution.y);
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{
	// 카메라 시점
	Vec2 vResolution = CEngine::Get()->GetResolution();
	m_LookAt = vResolution / 2.f;

	// Door텍스쳐
	m_Door1.DoorTexure = CAssetMgr::Get()->LoadTexture(L"Door1", L"texture\\start\\menuDoor01.png");
	m_Door2.DoorTexure = CAssetMgr::Get()->LoadTexture(L"Door2", L"texture\\start\\menuDoor02.png");


	// 카메라 스테이터스 초기화
	CameraStatus |= CAMERAINIT;
	m_DoorOn = true;


	// 카메라 도어 위치 설정
	m_DoorVelocity = Vec2(2600,0);
	m_Door1.DoorClosePos = Vec2(-10, 0);
	m_Door1.DoorScale = Vec2(m_Door1.DoorTexure->GetWidth() + 163.f, vResolution.y);
	m_Door1.DoorOpenPos = Vec2(-660, 0.f);
	m_Door1.DoorPos = Vec2(-10, 0);

	m_Door2.DoorClosePos = Vec2(vResolution.x / 5.f * 3.f - 205.f, 0);
	m_Door2.DoorScale = Vec2(m_Door2.DoorTexure->GetWidth() + 163.f, vResolution.y);
	m_Door2.DoorOpenPos = Vec2(vResolution.x / 5.f * 3.f + 470.f, 0.f);
	m_Door2.DoorPos = Vec2(vResolution.x / 5.f * 3.f - 205.f, 0);
}

void CCamera::Tick()
{
	// 카메라 동작 확인
	CameraMove();

	// 카메라 효과 동작
	CameraEffect();

	// 카메라가 보고있는 지점과, 해상도 중심위치의 차이값
	m_LookAtDiff = (m_LookAt + m_ShakeOffset) - (CEngine::Get()->GetResolution() / 2.f);


	// Door 동작
	if (0.f >= m_DoorHoldDuration)
	{
		// 우측 도어 열기
		if (m_DoorComand.size() != 0)
		{
			if (m_DoorComand.front() & COMMANDHALFOPEN)
			{
				DoorHalfOpen();
			}
			else if (m_DoorComand.front() & COMMANDALLOPEN)
			{
				DoorAllOpen();
			}
			else if (m_DoorComand.front() & COMMANDHALFCLOSE)
			{
				DoorHalfClose();
			}
			else if (m_DoorComand.front() & COMMANDALLCLOSE)
			{
				DoorAllClose();
			}
		}
	}
	// 현재 상태를 유지하고 싶다면
	else
	{
		m_DoorHoldDuration -= REALDT;
	}
}

void CCamera::Render()
{
	// 페이드 in/out
	FilterEffect();

	// 도어 효과
	DoorEffect();

	if (IsDoorMove())
	{
		CUIMgr::Get()->SetActiveUI(false);
	}
	else
	{
		CUIMgr::Get()->SetActiveUI(true);
	}
}

void CCamera::CameraEffect()
{
	if (false == m_CamShake)
		return;

	// 카메라 이동 방향(1.f == 아래, -1.f == 위)	
	m_ShakeOffset.x += m_Amplitude * 4.f * m_Frequency * DT * m_Dir;

	// 진폭 수치를 넘어서면 방향을 전환시켜준다.
	if (m_Amplitude < fabs(m_ShakeOffset.x))
	{
		// 초과량을 다시 안쪽으로 적용시킨다.
		m_ShakeOffset.x += (fabs(m_ShakeOffset.x) - m_Amplitude) * -m_Dir;

		// 방향 전환
		m_Dir *= -1.f;
	}

	// 효과 유지시간이 다 되었는지 체크
	m_AccTime += DT;
	if (m_Duration < m_AccTime)
	{
		// 효과가 만료되면 세팅 값 정리
		m_AccTime = 0.f;
		m_CamShake = false;
		m_ShakeOffset = Vec2(0.f, 0.f);
	}
}

void CCamera::FilterEffect()
{
	if (m_FilterInfo.empty())
		return;

	FILTER_INFO& info = m_FilterInfo.front();

	UINT Alpha = 0;
	if (FILTER_EFFECT::FADE_OUT == info.Effect)
	{
		float fRatio = info.AccTime / info.Duration;
		Alpha = (UINT)(255.f * fRatio);
	}

	else if (FILTER_EFFECT::FADE_IN == info.Effect)
	{
		float fRatio = 1.f - (info.AccTime / info.Duration);
		Alpha = (UINT)(255.f * fRatio);
	}

	info.AccTime += DT;
	if (info.Duration < info.AccTime)
	{
		m_FilterInfo.pop_front();
	}

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = Alpha; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = 0;

	AlphaBlend(BackDC, 0, 0
		, m_BlackFilterTex->GetWidth()
		, m_BlackFilterTex->GetHeight()
		, m_BlackFilterTex->GetDC()
		, 0, 0
		, m_BlackFilterTex->GetWidth()
		, m_BlackFilterTex->GetHeight()
		, blend);
}

void CCamera::CameraMove()
{
	// wasd로 조작, 카메라 고정기능 켜지면 작동 x
	if (KEY_PRESSED(KEY::W) && !(CameraStatus & CAMERAUPDOWNLOCK))
	{
		m_LookAt.y -= m_CamSpeed * DT;
	}
	if (KEY_PRESSED(KEY::S) && !(CameraStatus & CAMERAUPDOWNLOCK))
	{
		m_LookAt.y += m_CamSpeed * DT;
	}

	if (KEY_PRESSED(KEY::A) && !(CameraStatus & CAMERALRLOCK))
	{
		m_LookAt.x -= m_CamSpeed * DT;
	}

	if (KEY_PRESSED(KEY::D) && !(CameraStatus & CAMERALRLOCK))
	{
		m_LookAt.x += m_CamSpeed * DT;
	}

	// 카메라가 스테이지 모드일 경우 카메라 위치에 제한이 있을 것
	if (CameraStatus & STAGECAMERAMODE)
	{
		if (m_LookAt.x < m_CameraLMaxPos)
		{
			m_LookAt.x = m_CameraLMaxPos;
		}
		else if (m_LookAt.x > m_CameraRMaxPos)
		{
			m_LookAt.x = m_CameraRMaxPos;
		}

	}
}

void CCamera::DoorEffect()
{
	// DOORON상태일 경우에만 사용
	if (m_DoorOn)
	{
		// 해당 위치로 Door Render
		Vec2 vResolution = CEngine::Get()->GetResolution();
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
		blend.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(BackDC, m_Door1.DoorPos.x + m_ShakeOffset.x, m_Door1.DoorPos.y
			, m_Door1.DoorScale.x, m_Door1.DoorScale.y
			, m_Door1.DoorTexure->GetDC()
			, 0, 0
			, m_Door1.DoorTexure->GetWidth()
			, m_Door1.DoorTexure->GetHeight()
			, blend);

		AlphaBlend(BackDC, m_Door2.DoorPos.x + m_ShakeOffset.x, m_Door2.DoorPos.y
			, m_Door2.DoorScale.x, m_Door2.DoorScale.y
			, m_Door2.DoorTexure->GetDC()
			, 0, 0
			, m_Door2.DoorTexure->GetWidth()
			, m_Door2.DoorTexure->GetHeight()
			, blend);
	}
}

void CCamera::DoorHalfOpen()
{
	// 만일 좌측 도어 위치가 Close위치가 아닐 시
	if (m_Door1.DoorPos.x < m_Door1.DoorClosePos.x)
	{
		m_Door1.DoorPos.x = m_Door1.DoorClosePos.x;
	}

	// 우측 도어 오측으로 구동
	m_Door2.DoorPos += m_DoorVelocity * REALDT;
	// 최종 목적지에 도달하면 명령제거, 절반 열린 상태로 보게 한다.
	if (m_Door2.DoorPos.x > m_Door2.DoorOpenPos.x)
	{
		m_Door2.DoorPos.x = m_Door2.DoorOpenPos.x;
		CameraStatus &= ~COMMANDHALFOPEN;
		CameraStatus |= DOORHALFCLOSE;
		m_DoorComand.pop_front();
		m_DoorHoldDuration = 0.2f;
	}


}

void CCamera::DoorHalfClose()
{
	// 만일 좌측 도어 위치가 Open위치가 아닐 시
	if (m_Door1.DoorPos.x < m_Door1.DoorOpenPos.x)
	{
		m_Door1.DoorPos.x = m_Door1.DoorOpenPos.x;
	}

	// 우측 도어 좌측으로 구동
	m_Door2.DoorPos -= m_DoorVelocity * REALDT;
	// 최종 목적지에 도달하면 명령제거, 닽힌 상태로 보게 한다.
	if (m_Door2.DoorPos.x < m_Door2.DoorClosePos.x)
	{
		m_Door2.DoorPos.x = m_Door2.DoorOpenPos.x;
		CameraStatus &= ~COMMANDHALFCLOSE;
		CameraStatus |= DOORALLCLOSE;
		CameraStatus |= DOORHALFCLOSE;
		m_DoorComand.pop_front();
		m_DoorHoldDuration = 0.2f;
		SetCamShake(10.f, 10.f, 0.1f);
		
	}
}

void CCamera::DoorAllOpen()
{

	// 우측 도어 오측으로 구동
	if (m_Door2.DoorPos.x < m_Door2.DoorOpenPos.x)
	m_Door2.DoorPos += m_DoorVelocity * REALDT;

	//좌측 도어 좌측으로 구동
	if (m_Door1.DoorPos.x > m_Door1.DoorOpenPos.x)
	m_Door1.DoorPos -= m_DoorVelocity * REALDT;
	// 최종 목적지에 도달하면 명령제거, 모두 열린 상태로 보게 한다.
	if (m_Door1.DoorPos.x <= m_Door1.DoorOpenPos.x && m_Door2.DoorPos.x >= m_Door2.DoorOpenPos.x)
	{
		m_Door1.DoorPos.x = m_Door1.DoorOpenPos.x;
		m_Door2.DoorPos.x = m_Door2.DoorOpenPos.x;
		CameraStatus &= ~COMMANDALLOPEN;
		CameraStatus |= DOORALLOPEN;
		m_DoorComand.pop_front();
		m_DoorHoldDuration = 0.2f;
	}
}

void CCamera::DoorAllClose()
{
	// 우측 도어 좌측으로 구동
	if (m_Door2.DoorPos.x > m_Door2.DoorClosePos.x)
		m_Door2.DoorPos -= m_DoorVelocity * REALDT;

	// 좌측 도어 우측으로 구동
	if (m_Door1.DoorPos.x < m_Door1.DoorClosePos.x)
		m_Door1.DoorPos += m_DoorVelocity * REALDT;
	// 최종 목적지에 도달하면 명령제거, 모두 닽힌 상태로 보게 한다.
	if (m_Door1.DoorPos.x >= m_Door1.DoorClosePos.x && m_Door2.DoorPos.x <= m_Door2.DoorClosePos.x)
	{
		m_Door1.DoorPos.x = m_Door1.DoorClosePos.x;
		m_Door2.DoorPos.x = m_Door2.DoorClosePos.x;
		CameraStatus &= ~COMMANDALLCLOSE;
		CameraStatus |= DOORALLCLOSE;
		m_DoorComand.pop_front();
		m_DoorHoldDuration = 0.2f;
		SetCamShake(10.f, 10.f, 0.1f);
	}
}


void CCamera::SetCameraStatus(unsigned int _State)
{
		CameraStatus |= _State;
		m_DoorComand.push_back(_State);
		if (_State & COMMANDALLCLOSE || _State & COMMANDHALFCLOSE || _State & COMMANDALLOPEN || _State & COMMANDHALFOPEN)
		{
			CameraStatus &= ~DOORALLCLOSE;
			CameraStatus &= ~DOORHALFCLOSE;
			CameraStatus &= ~DOORALLOPEN;
			CameraStatus &= ~DOORHALFOPEN;
		}
}

void CCamera::SetCameraUPDOWNLock(bool _true)
{
	if(_true)
		CameraStatus |= CAMERAUPDOWNLOCK;
	else
		CameraStatus &= ~CAMERAUPDOWNLOCK;
}

void CCamera::SetCameraLRLock(bool _true)
{
	if (_true)
		CameraStatus |= CAMERALRLOCK;
	else
		CameraStatus &= ~CAMERALRLOCK;
}

void CCamera::SetCameraStageMode(bool _true)
{
	if (_true)
		CameraStatus |= STAGECAMERAMODE;
	else
		CameraStatus &= ~STAGECAMERAMODE;
}

void CCamera::SetStageCamerMaxPos(float _x1, float _x2)
{
	if (_x1 < _x2)
	{
		m_CameraLMaxPos = _x1; 
		m_CameraRMaxPos = _x2;
	}
	else
	{
		m_CameraLMaxPos = _x2; 
		m_CameraRMaxPos = _x1;
	}
}

bool CCamera::IsDoorMove()
{
	if (CameraStatus & COMMANDALLCLOSE || CameraStatus & COMMANDHALFCLOSE || CameraStatus & COMMANDALLOPEN || CameraStatus & COMMANDHALFOPEN)
	{
		return true;
	}


	return false;
}

