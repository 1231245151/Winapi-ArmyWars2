#include "pch.h"
#include "TraceState.h"

#include "CUnit.h"
#include "CBunker.h"

TraceState::TraceState()
{
}

TraceState::~TraceState()
{
}

void TraceState::Enter()
{
}

void TraceState::FinalTick()
{
	CUnit* OwnerUnit = GetOwner<CUnit>();
	if (false == OwnerUnit->GetMovestat())
	{
		m_OwnerPos = OwnerUnit->GetPos();
		m_OwnerPos += (OwnerUnit->GetStat().velocity * DT);
		OwnerUnit->SetPos(m_OwnerPos);
	}

	//wchar_t piStr[20];
	//swprintf(piStr, 20, L"%.3f", m_OwnerPos);
	//LOG(LOG_LEVEL::WARNING, piStr);


	// 타겟 정보가 있다면 ShootState로 변경하여 공격 개시
	vector<CObj*>& ptarget = OwnerUnit->GetTargetObj();

	if (!ptarget.empty())
	{
		GetStateMachine()->ChangeState(L"ShootState");
	}
}

void TraceState::Exit()
{
}
