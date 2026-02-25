#include "pch.h"
#include "CLogMgr.h"

#include "CEngine.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

CLogMgr::CLogMgr()
	: m_LogMaxLife(3.f)
	, m_LogStep(18)
	, m_LogOnOff(true)
{
}

CLogMgr::~CLogMgr()
{

}

void CLogMgr::Tick()
{
	if (KEY_TAP(KEY::SPACE))
	{
		m_LogOnOff ? m_LogOnOff = false : m_LogOnOff = true;
	}

	list<tLog>::iterator iter = m_LogList.begin();

	int i = 0;
	for (iter; iter != m_LogList.end(); ++i)
	{
		if (m_LogOnOff)
		{
			// 로그를 화면에 출력시킨다.
			Vec2 vLogPos = Vec2(10.f, 10.f + (float)(i * m_LogStep));

			// 로그 수준에따라 색상을 다르게 한다.
			switch ((*iter).Level)
			{
			case LOG_LEVEL::LOG:
				SetTextColor(BackDC, RGB(220, 220, 220));
				break;
			case LOG_LEVEL::WARNING:
				SetTextColor(BackDC, RGB(220, 220, 30));
				break;
			case LOG_LEVEL::BUG:
				SetTextColor(BackDC, RGB(240, 30, 30));
				break;
			}
			SetTextAlign(BackDC, TA_TOP);
			SetBkMode(BackDC, TRANSPARENT);
			TextOut(BackDC, (int)vLogPos.x, (int)vLogPos.y, (*iter).strLog.c_str(), (*iter).strLog.length());
		}

		// 로그의 수명이 다 끝났으면 리스트에서 제거한다.
		(*iter).Time += DT;
		if (m_LogMaxLife < (*iter).Time)
		{
			iter = m_LogList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
