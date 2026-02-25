#include "pch.h"
#include "BaseTraceState.h"

#include "CBunker.h"
#include "CUnit.h"

BaseTraceState::BaseTraceState()
{
}

BaseTraceState::~BaseTraceState()
{
}


void BaseTraceState::Enter()
{
}

void BaseTraceState::FinalTick()
{
	CBunker* OwnerBase = GetOwner<CBunker>();
	m_OwnerPos = OwnerBase->GetPos();
	CUnit* targetUnit = nullptr;
	float MinDist = 10000;

	vector<CObj*>& ptarget = OwnerBase->GetTargetObj();

	if (!ptarget.empty())
	{
		vector<CObj*>::iterator iter = ptarget.begin();

		// 가장 가까운 유닛 탐색
		for (; iter != ptarget.end(); ++iter)
		{
			// x축 거리 계산
			float vDist = abs((*iter)->GetPos().x - m_OwnerPos.x);

			// x축 거리가 가장 짧을 Unit
			if (vDist < MinDist)
			{
				CUnit* IsUnit = (CUnit*)*iter;

				// 체력이 0이 아닌 유닛을 계산
				if (0 < IsUnit->GetStat().HP)
				{
					MinDist = vDist;
					targetUnit = (CUnit*)*iter;
				}

			}
		}

		// 해당 유닛에게 사격
		if (nullptr != targetUnit)
		{
			if (OwnerBase->GetBaseStat().AttackSpeed < OwnerBase->GetBaseStat().AttackDuration)
			{
				OwnerBase->Demagecall(targetUnit->GetStat(), targetUnit);
				OwnerBase->GetBaseStat().AttackDuration = 0.f;
			}
		}


	}

}

void BaseTraceState::Exit()
{
}