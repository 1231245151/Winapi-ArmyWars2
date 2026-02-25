#pragma once

#define DOORHALFOPEN					0x02		// 현재 문상태 : 절반 열림
#define DOORHALFCLOSE					0x04		// 현재 문상태 : 절반 닫힘(사실상 기능없음)
#define DOORALLOPEN						0x08		// 현재 문상태 : 모두 닫힘 
#define DOORALLCLOSE					0x10		// 현재 문상태 : 모두 닫힘
#define COMMANDALLCLOSE					0x20		// 명령 : 모두 닫기
#define COMMANDHALFCLOSE				0x40		// 명령 : 절반 닫기
#define COMMANDALLOPEN					0x80		// 명령 : 모두 열기
#define COMMANDHALFOPEN					0x100		// 명령 : 절반 열기

#define CAMERAUPDOWNLOCK				0x1000		// 카메라 상하 구동 Lock
#define CAMERALRLOCK					0x2000		// 카메라 좌우 구동 Lock
#define STAGECAMERAMODE					0x4000		// 스테이지 시 카메라 제한 기능 On

#define CAMERAINIT (CAMERAUPDOWNLOCK | CAMERALRLOCK)
		

class CTexture;

enum FILTER_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct FILTER_INFO
{
	FILTER_EFFECT	Effect;
	float			Duration;
	float			AccTime;
};

struct DOOR_EFFECT
{
	CTexture* DoorTexure;
	Vec2	  DoorScale;
	Vec2	  DoorPos;
	Vec2	  DoorOpenPos;
	Vec2	  DoorClosePos;
};


class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_LookAt;		// 카메라가 보고있는 실제위치
	Vec2		m_LookAtDiff;	// 해상도 중심과, m_LookAt 과의 차이값을 계산 (실제 위치->렌더포즈, 렌더포즈->실제위치)
	Vec2		m_ShakeOffset;
	float		m_CamSpeed;

	unsigned int CameraStatus;		// 카메라 상태

	// Door 상태
	DOOR_EFFECT m_Door1;
	DOOR_EFFECT m_Door2;
	Vec2		m_DoorVelocity;
	float		m_DoorHoldDuration;
	list<unsigned int> m_DoorComand;		// 중첩으로 명령 받을 시 대비용
	bool		m_DoorOn;

	// Filter Effect
	CTexture* m_BlackFilterTex;
	list<FILTER_INFO>	m_FilterInfo;

	// Camera Oscillation
	float		m_Frequency;
	float		m_Amplitude;
	float		m_Duration;
	float		m_AccTime;
	float		m_Dir;
	bool		m_CamShake;

	float		m_CameraLMaxPos;
	float		m_CameraRMaxPos;

public:
	// Oscillation
	void SetCamShake(float _Frequency, float _Amplitude, float _Duration)
	{
		m_Frequency = _Frequency;
		m_Amplitude = _Amplitude;
		m_Duration = _Duration;
		m_Dir = 1.f;
		m_AccTime = 0.f;
		m_CamShake = true;
	}

	void SetFilterEffect(FILTER_EFFECT _Effect, float _Duration)
	{
		FILTER_INFO info = {};

		info.Effect = _Effect;
		info.Duration = _Duration;
		info.AccTime = 0.f;

		m_FilterInfo.push_back(info);
	}

	void SetLookAt(Vec2 _Look) { m_LookAt = _Look; }
	Vec2 GetLookAt() { return m_LookAt + m_ShakeOffset; }
	Vec2 GetRenderPos(Vec2 _RealPos) { return _RealPos - m_LookAtDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_LookAtDiff; }
	CTexture* GetFilterEffect() { return m_BlackFilterTex; }

public:
	unsigned int GetCameraStat() { return CameraStatus; }
	void SetCameraStatus(unsigned int _State);
	void SetCameraUPDOWNLock(bool _true);
	void SetCameraLRLock(bool _true);
	void SetCameraStageMode(bool _true);

	void SetStageCamerMaxPos(float _x1, float _x2);
	void SetDoorDuration(float _Duration) { m_DoorHoldDuration = _Duration; }
	void SetDoorOn(bool _true) { m_DoorOn = _true; }

	bool IsDoorMove();

public:
	void Init();
	void Tick();
	void Render();

private:
	void CameraEffect();
	void FilterEffect();
	void CameraMove();

	void DoorEffect();
	void DoorHalfOpen();
	void DoorHalfClose();
	void DoorAllOpen();
	void DoorAllClose();
};

