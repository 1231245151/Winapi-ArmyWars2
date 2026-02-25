#include "pch.h"
#include "DeathState.h"

#include "CUnit.h"
#include "CUnitMgr.h"
#include "CBunker.h"
#include "CLevelMgr.h"


DeathState::DeathState()
	:m_DeathDuration(1.f)
{
}

DeathState::~DeathState()
{
}

void DeathState::Enter()
{
	CUnit* pUnit = GetOwner<CUnit>();
	pUnit->PlayAnimation(L"Death", false);
	pUnit->PlayDeathSound();

	// 사망 상태가되면 기지의 코스트를 회복을 시킨다
	if ((int)LAYER_TYPE::ENEMY_UNIT == (int)(pUnit->GetLayerType()))
	{
		// 현재 레벨에 있는 아군 벙커 찾기(1개뿐일 테니 배열의 0번째)
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		vector<CObj*>& CurVec = CurLevel->GetLayer(LAYER_TYPE::PLAYER_BASE);
		CBunker* targetBunker = dynamic_cast<CBunker*>(CurVec[0]);

		targetBunker->GetHaveFUND() += (pUnit->GetStat().FUND * 1.2f);
		// 초과하면 최대 FUND만큼만 보유
		if (targetBunker->GetHaveFUND() > targetBunker->GetMaxFUND())
			targetBunker->GetHaveFUND() = targetBunker->GetMaxFUND();
	}

	// 킬카운터 올리기
	if ((int)LAYER_TYPE::ENEMY_UNIT == (int)(pUnit->GetLayerType()))
	{
		CUnitMgr::Get()->IncreaseEkillCount(pUnit->GetEUnitName());
	}
}

void DeathState::FinalTick()
{
	CUnit* pUnit = GetOwner<CUnit>();
	m_DeathDuration -= DT;

	// 사망 지속시간이 끝나면 삭제처리
	if (m_DeathDuration < 0.f)
	{
		DeleteObject(pUnit);
	}
}

void DeathState::Exit()
{
}