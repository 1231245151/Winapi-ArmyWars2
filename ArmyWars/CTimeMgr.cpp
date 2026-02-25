#include "pch.h"
#include "CTimeMgr.h"
#include "CEngine.h"


CTimeMgr::CTimeMgr()
	:m_IsTimeStop(false)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurrentCount);

	m_DT = (double)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (double)m_Frequency.QuadPart;
	m_fDT = (float)m_DT;

	// 시간 누적
	m_Time += m_DT;

	// 시간 누적
	m_Second += m_DT;

	// 함수 호출횟수
	++m_FPS;

	// 실제 DT값
	m_RealfDT = m_fDT;

	// 60 FPS미만으로 떨어지면 프레임 강제 고정
	if (m_fDT > 1.f / 60.f)
	{
		m_fDT = 1.f / 60.f;
	}

	if (m_RealfDT > 1.f / 60.f)
	{
		m_RealfDT = 1.f / 60.f;
	}

	if (1. < m_Second)
	{
		m_Second -= 1.;

		// 윈도우 타이틀에 FPS 랑 DeltaTime 표시
		wchar_t buff[256] = {};
		swprintf_s(buff, 256, L"FPS : %d, DeltaTime : %f", m_FPS, m_RealfDT);
		SetWindowText(CEngine::Get()->GetMainHwnd(), buff);

		m_FPS = 0;
	}

	m_PrevCount = m_CurrentCount;

	if (m_IsTimeStop)
	{
		m_fDT = 0.f;
		m_DT = 0.f;
	}
}
