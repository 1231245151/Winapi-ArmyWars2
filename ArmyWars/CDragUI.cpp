#include "pch.h"
#include "CDragUI.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CLogMgr.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CAnimator.h"

CDragUI::CDragUI()
	: m_CallBack(nullptr)
	, m_Inst(nullptr)
	, m_MemFunc(nullptr)
	, m_MemFunc_1(nullptr)
	, m_MemFunc_2(nullptr)
	, m_NormalImg(nullptr)
	, m_PressedImg(nullptr)
	, m_OffImg(nullptr)
	, m_Animation(nullptr)
	, m_DragOn(false)
	, m_OriginalPos(Vec2(0.f,0.f))
{
}

CDragUI::~CDragUI()
{
}

void CDragUI::Tick_UI()
{
	if (m_OriginalPos == Vec2(0.f, 0.f))
	{
		m_OriginalPos = GetPos();
	}

	if (m_BtnOn)
	{
		if (IsLBtnDown() && IsActive())
		{
			Vec2 vCurMousePos = CKeyMgr::Get()->GetMousePos();
			Vec2 vDiff = vCurMousePos - m_GrabPos;


			Vec2 MouseRealPos = CCamera::Get()->GetRealPos(CKeyMgr::Get()->GetMousePos());
			SetPos(MouseRealPos);

			m_GrabPos = vCurMousePos;
		}
		else if (!IsActive() && IsLBtnDown())
		{
			LBtnUp();
			SetLBtnDown(false);
		}
	}
}

void CDragUI::Render_UI()
{
	// 버튼 On이며 애니메이션이 존재하며 드래그 상태일 경우에 애니메이션 사용
	if (nullptr != m_Animation && m_BtnOn && IsActive() && m_DragOn)
	{
		// 애니메이션에게 랜더링 위치로 맞추라고 보냄
		m_Animation->SetIsPos(true);
		m_Animation->Render();

	}

	CTexture* RenderImg = nullptr;
	Vec2 RenderPos = Vec2(0.f, 0.f);
	Vec2 RenderScale = Vec2(0.f, 0.f);
	Vec2 RenderOffset = Vec2(0.f, 0.f);
	Vec2 RenderRealScale = Vec2(0.f, 0.f);
	UINT RenderAlpha = 255;
	Vec2 vFinalPos = GetFinalPos();

	if ((m_BtnOn && !IsLBtnDown()))
	{
		RenderImg = m_NormalImg;
		RenderPos = m_NorLTopPos;
		RenderScale = m_NorScale;
		RenderOffset = m_NorOffSetPos;
		RenderRealScale = m_NorRenderScale;
		RenderAlpha = m_NorAlpha;
	}
	else if (m_BtnOn && IsActive())
	{
		RenderImg = m_PressedImg;
		RenderPos = m_PresseLTopPos;
		RenderScale = m_PresseScale;
		RenderOffset = m_PresseOffSetPos;
		RenderRealScale = m_PresseRenderScale;
		RenderAlpha = m_PresseAlpha;
		vFinalPos = CKeyMgr::Get()->GetMousePos();
	}
	else if(!m_BtnOn)
	{
		RenderImg = m_OffImg;
		RenderPos = m_OffLTopPos;
		RenderScale = m_OffScale;
		RenderOffset = m_OffOffSetPos;
		RenderRealScale = m_OffRenderScale;
		RenderAlpha = m_OffAlpha;
	}

	if (nullptr == RenderImg)
	{
		if (nullptr == m_NormalImg)
		{
			return;
		}
		else
		{
			RenderImg = m_NormalImg;
		}
	}

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = RenderAlpha; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;


	AlphaBlend(BackDC, (int)vFinalPos.x + RenderOffset.x, (int)vFinalPos.y + RenderOffset.y
		, RenderRealScale.x, RenderRealScale.y
		, RenderImg->GetDC()
		, RenderPos.x, RenderPos.y
		, RenderScale.x, RenderScale.y
		, blend);

	//wstring Msg = std::
	// (vFinalPos.y);
	//LOG(LOG_LEVEL::LOG, Msg.c_str())
}

void CDragUI::RenderTexture()
{
}


void CDragUI::LBtnDown()
{
	if (m_BtnOn)
	{
		m_DragOn = true;
		m_GrabPos = CKeyMgr::Get()->GetMousePos();
		SetPriority(true);
	}
}

void CDragUI::LBtnUp()
{
	if (m_BtnOn)
	{
		m_DragOn = false;
		SetPos(m_OriginalPos);
		SetPriority(false);
	}
}

void CDragUI::LBtnClicked()
{
	if (m_BtnOn && IsActive())
	{
		if (m_CallBack)
		{
			m_CallBack();
		}

		if (m_Inst && m_MemFunc)
		{
			(m_Inst->*m_MemFunc)();
		}

		if (m_Inst && m_MemFunc_1)
		{
			(m_Inst->*m_MemFunc_1)(m_DWORD1);
		}
	}
}


void CDragUI::SetAnimation(CAnimator* _Animator)
{
	m_Animation = AddComponent<CAnimator>(_Animator);
	m_Animation->SetOffset(Vec2(0.f,0.f));
}

void CDragUI::SetNormalImg(CTexture* _Atlas, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_NormalImg = _Atlas;
	m_NorLTopPos = Vec2(0, 0);
	m_NorScale = Vec2(m_NormalImg->GetWidth(), m_NormalImg->GetHeight());
	m_NorOffSetPos = _Offset;
	m_NorAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_NorRenderScale = _RenderScale;
	else
		m_NorRenderScale = GetScale();
}

void CDragUI::SetPressedImg(CTexture* _Atlas, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_PressedImg = _Atlas;
	m_PresseLTopPos = Vec2(0, 0);
	m_PresseScale = Vec2(m_PressedImg->GetWidth(), m_PressedImg->GetHeight());
	m_PresseOffSetPos = _Offset;
	m_PresseAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_PresseRenderScale = _RenderScale;
	else
		m_PresseRenderScale = GetScale();
}

void CDragUI::SetOffImg(CTexture* _Atlas, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_OffImg = _Atlas;
	m_OffLTopPos = Vec2(0, 0);
	m_OffScale = Vec2(m_OffImg->GetWidth(), m_OffImg->GetHeight());
	m_OffOffSetPos = _Offset;
	m_OffAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_OffRenderScale = _RenderScale;
	else
		m_OffRenderScale = GetScale();
}

void CDragUI::SetNormalImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_NormalImg = _Atlas;
	m_NorLTopPos = _LTopPos;
	m_NorScale = _Scale;
	m_NorOffSetPos = _Offset;
	m_NorAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_NorRenderScale = _RenderScale;
	else
		m_NorRenderScale = GetScale();
}

void CDragUI::SetPressedImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_PressedImg = _Atlas;
	m_PresseLTopPos = _LTopPos;
	m_PresseScale = _Scale;
	m_PresseOffSetPos = _Offset;
	m_PresseAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_PresseRenderScale = _RenderScale;
	else
		m_PresseRenderScale = GetScale();
}

void CDragUI::SetOffImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset, UINT _Alpha, Vec2 _RenderScale)
{
	m_OffImg = _Atlas;
	m_OffLTopPos = _LTopPos;
	m_OffScale = _Scale;
	m_OffOffSetPos = _Offset;
	m_OffAlpha = _Alpha;
	if (Vec2(0.f, 0.f) != _RenderScale)
		m_OffRenderScale = _RenderScale;
	else
		m_OffRenderScale = GetScale();
}