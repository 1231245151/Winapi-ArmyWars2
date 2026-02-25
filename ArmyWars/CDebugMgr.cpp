#include "pch.h"
#include "CDebugMgr.h"

#include "CTimeMgr.h"
#include "CEngine.h"
#include "CKeyMgr.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CUnit.h"
#include "CBunker.h"
#include "CObj.h"
#include "CUnitMgr.h"
#include "CSkill.h"

CDebugMgr::CDebugMgr()
	: m_DbgOnOff(true)
{

}

CDebugMgr::~CDebugMgr()
{

}


void CDebugMgr::Tick()
{

	if (KEY_TAP(KEY::SPACE))
	{
		m_DbgOnOff ? m_DbgOnOff = false : m_DbgOnOff = true;
	}

	// 치트키 탐색
	CheatKey();

#ifdef _DEBUG
	// 디버그 쉐이프를 그릴 DC 를 가져온다(백버퍼 DC)
	HDC hBackDC = CEngine::Get()->GetBackDC();


	// 디버그 쉐이프 정보들을 모두 그리기위해서 반복문을 돈다.
	list<tDebugShapeInfo>::iterator iter = m_DbgList.begin();
	for (; iter != m_DbgList.end(); )
	{
		if (m_DbgOnOff)
		{
			// 백버퍼 DC 가 요청된 펜을 선택하게 한다.
			SELECT_PEN(hBackDC, (*iter).Pen);

			// 백버퍼 DC 가 요청된 브러쉬를 선택하게 한다.
			SELECT_BRUSH(hBackDC, (*iter).BRUSH);
			switch ((*iter).Shape)
			{
			case DEBUG_SHAPE::RECT:
				Rectangle(hBackDC
					, (*iter).Position.x - (*iter).Scale.x / 2.f
					, (*iter).Position.y - (*iter).Scale.y / 2.f
					, (*iter).Position.x + (*iter).Scale.x / 2.f
					, (*iter).Position.y + (*iter).Scale.y / 2.f);
				break;
			case DEBUG_SHAPE::CIRCLE:
				Ellipse(hBackDC
					, (*iter).Position.x - (*iter).Scale.x / 2.f
					, (*iter).Position.y - (*iter).Scale.y / 2.f
					, (*iter).Position.x + (*iter).Scale.x / 2.f
					, (*iter).Position.y + (*iter).Scale.y / 2.f);
				break;
			case DEBUG_SHAPE::CROSS:
				MoveToEx(hBackDC, (*iter).Position.x - (*iter).Scale.x / 2.f, (*iter).Position.y, nullptr);
				LineTo(hBackDC, (*iter).Position.x + (*iter).Scale.x / 2.f, (*iter).Position.y);

				MoveToEx(hBackDC, (*iter).Position.x, (*iter).Position.y - (*iter).Scale.y / 2.f, nullptr);
				LineTo(hBackDC, (*iter).Position.x, (*iter).Position.y + (*iter).Scale.y / 2.f);
				break;
			case DEBUG_SHAPE::LINE:
				MoveToEx(hBackDC, (*iter).Position.x, (*iter).Position.y, nullptr);
				LineTo(hBackDC, (*iter).Scale.x, (*iter).Scale.y);
				break;
			}
		}



		// 시간을 체크하다가, DebugShape 유지시간을 넘기면, 리스트에서 정보를 제거한다.
		(*iter).Time += DT;
		if ((*iter).Duration < (*iter).Time)
		{
			iter = m_DbgList.erase(iter);
		}
		else
		{
			++iter;
		}

	}
#endif
}

void CDebugMgr::CheatKey()
{
	// 치트 : 적 유닛 몰살
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	if (KEY_TAP(KEY::KEY_4))
	{
		vector<CObj*>& VecObj = CurLevel->GetLayer(LAYER_TYPE::ENEMY_UNIT);

		for (int i = 0; i < VecObj.size(); ++i)
		{
			CUnit* CurUnit = dynamic_cast<CUnit*>(VecObj[i]);
			CurUnit->GetStat().HP = 0.f;
		}

	}

	// 치트 : 코스트 풀로 회복
	if (KEY_TAP(KEY::KEY_1))
	{
		vector<CObj*>& VecObj = CurLevel->GetLayer(LAYER_TYPE::PLAYER_BASE);
		if (VecObj.size() > 0)
		{
			CBunker* CurBunker = dynamic_cast<CBunker*>(VecObj[0]);

			CurBunker->GetHaveFUND() = CurBunker->GetMaxFUND();
		}
	}

	// 치트 : 유닛 쿨타임 크게 감소
	if (KEY_TAP(KEY::KEY_2))
	{
		vector<CUnit*>& CurUnit = CUnitMgr::Get()->GetPSummonIndx();

		for (int i = 0; i < CurUnit.size(); ++i)
		{
			CurUnit[i]->GetStat().CooltimeDuration = 0.f;
		}
	}


	// 치트 : 적 유닛 현재 쿨타임 30초 감소 설정
	if (KEY_TAP(KEY::KEY_3))
	{
		vector<CUnit*>& CurUnit = CUnitMgr::Get()->GetESummonIndx();

		for (int i = 0; i < CurUnit.size(); ++i)
		{
			CurUnit[i]->GetStat().CooltimeDuration -= 30.f;
		}
	}

	// 치트 : 스킬 쿨타임 초기화
	if (KEY_TAP(KEY::KEY_2))
	{
		CUnitMgr::Get()->GetSkill(SKILL_NAME::REPAIR)->GetStat().CooltimeDuration = 0.f;
		CUnitMgr::Get()->GetSkill(SKILL_NAME::AIRSTRIKE)->GetStat().CooltimeDuration = 0.f;
		CUnitMgr::Get()->GetSkill(SKILL_NAME::PRECISIONBOOM)->GetStat().CooltimeDuration = 0.f;
	}
}