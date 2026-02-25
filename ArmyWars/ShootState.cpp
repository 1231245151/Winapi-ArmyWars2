#include "pch.h"
#include "ShootState.h"

#include "CUnit.h"
#include "CBunker.h"

ShootState::ShootState()
{
}

ShootState::~ShootState()
{
}

void ShootState::Enter()
{
	CUnit* pUnit = GetOwner<CUnit>();
	m_OwnerPos = pUnit->GetPos();
}

void ShootState::FinalTick()
{
	CUnit* OwnerUnit = GetOwner<CUnit>();
	Unit_Stat& Ownerstat = OwnerUnit->GetStat();

	// 속도 값 만큼 이동 처리
	if (false == OwnerUnit->GetMovestat())
	{
		m_OwnerPos += (OwnerUnit->GetStat().velocity * DT);
		OwnerUnit->SetPos(m_OwnerPos);
	}

	// 부모에 등록된 공격할 타겟중 가까운 대상을 탐색
	vector<CObj*>& ptarget = OwnerUnit->GetTargetObj();

	// 조건 검색용 변수
	float MinDist = 10000;
	float MaxHPRate = 1.f;

	CUnit*		targetUnit = nullptr;
	CUnit*		targetOwner = nullptr;
	CBunker*	targetbase = nullptr;

	vector<CObj*>::iterator iter = ptarget.begin();
	for (; iter != ptarget.end(); ++iter)
	{
		// 대상이 기지인지 확인
		int LayerType = (*iter)->GetLayerType();
		if ((int)LAYER_TYPE::PLAYER_BASE == LayerType || (int)LAYER_TYPE::ENEMY_BASE == LayerType)
		{
			targetbase = (CBunker*)*iter;
			continue;
		}

		// 기지가 아니니 유닛으로 보고 판단시작
		CUnit* IsUnit = (CUnit*)*iter;
		// 만일 충돌체 컴포넌트 없으면 검사안해줌
		CCollider* targetCol = IsUnit->GetComponent<CCollider>();
		CCollider* OwnerCol = OwnerUnit->GetComponent< CCollider>();
		if (nullptr == targetCol || nullptr == OwnerCol)
		{
			continue;
		}

		// 주인의 공격방식이 치유 공격인지 확인
		else if (UNIT_ATTACK::HEAL == OwnerUnit->GetShootType())
		{
			CUnit* IsUnit = (CUnit*)*iter;

			// 체력 비율 계산
			float vRate = (float)IsUnit->GetStat().HP / (float)IsUnit->GetStat().MaxHP;

			// 체력 비율이 가장 짧은 유닛
			if (vRate < MaxHPRate)
			{

				// 체력이 0이 아닌 유닛, 최대체력이 체력보다 크면 계산
				if (0 < IsUnit->GetStat().HP && (IsUnit->GetStat().MaxHP > IsUnit->GetStat().HP))
				{
					// 대상을 향한 공격력이 0이라면 타겟으로 삼지 않음
					if ((UNIT_TYPE::HUMEN == IsUnit->GetStat().Type) && (0 == Ownerstat.HDemage))
					{
						continue;
					}
					else if ((UNIT_TYPE::MACHINE == IsUnit->GetStat().Type) && (0 == Ownerstat.MDemage))
					{
						continue;
					}
					else
					{
						MaxHPRate = vRate;
						targetUnit = (CUnit*)*iter;
					}

				}

			}

			
		}

		// 치유가 아니면 공격 유닛일것
		else
		{

			// x축 거리 계산
			float vDist = abs(OwnerCol->GetFinalPos().x - targetCol->GetFinalPos().x);

			// x축 거리가 가장 짧을 Unit,
			if (vDist < MinDist)
			{

				// 체력이 0이 아닌 유닛을 계산, 
				if (0 < IsUnit->GetStat().HP)
				{
					// 대상을 향한 공격력이 0이라면 타겟으로 삼지 않음
					if ((UNIT_TYPE::HUMEN == IsUnit->GetStat().Type) && (0 == Ownerstat.HDemage))
					{

					}
					else if ((UNIT_TYPE::MACHINE == IsUnit->GetStat().Type) && (0 == Ownerstat.MDemage))
					{

					}
					else
					{
						MinDist = vDist;
						targetUnit = (CUnit*)*iter;
					}
				}

			}

		}
	}




	// 유닛이 존재하면 유닛에게 발사
	if (nullptr != targetUnit)
	{
		Unit_Stat& targetstat = targetUnit->GetStat();

		// 공격타입이 총알 발사 타입
		if (UNIT_ATTACK::SHOOT == OwnerUnit->GetShootType())
		{
			// 각도 계산
			Vec2 vDir = targetUnit->GetPos() - OwnerUnit->GetBulletShootPos();
			vDir.Normalize();

			// 해당 각도로 발사하라고 주인함수에게 보냄
			OwnerUnit->ShootBullet(vDir);
		}
		// 공격타입이 인스턴스 타입, 힐링도 인스턴스 타입임
		else
		{
			// 피해 계산은 피해를 입는 대상유닛측의 함수 출력
			// 다만, 발사 속도에 맞춰 출력 시킴
			if (Ownerstat.AttackSpeed < Ownerstat.AttackDuration)
			{
				OwnerUnit->Demagecall(targetUnit->GetStat(), targetUnit);
				Ownerstat.AttackDuration = 0.f;

				OwnerUnit->PlayShootSound();
			}
		}

	}

	// 유닛이 없다면 기지를 공격
	else if (nullptr != targetbase)
	{
		Unit_Stat& targetstat = targetbase->GetBaseStat();
		Unit_Stat& Ownerstat = OwnerUnit->GetStat();

		// 기지에게의 공격타입은 인스턴스 타입으로 고정
		if (Ownerstat.AttackSpeed < Ownerstat.AttackDuration)
		{
			OwnerUnit->Demagecall(targetbase->GetBaseStat(), targetbase);
			Ownerstat.AttackDuration = 0.f;
			OwnerUnit->PlayShootSound();
		}
	}

	// 만일 유일한 타겟이 자기자신이라면 자기자신에게
	else if (nullptr != targetOwner)
	{
		Unit_Stat& targetstat = targetOwner->GetStat();

		// 공격타입이 총알 발사 타입
		if (UNIT_ATTACK::SHOOT == OwnerUnit->GetShootType())
		{
			// 각도 계산
			Vec2 vDir = targetOwner->GetPos() - OwnerUnit->GetPos();
			vDir.Normalize();

			// 해당 각도로 발사하라고 주인함수에게 보냄
			OwnerUnit->ShootBullet(vDir);
		}
		// 공격타입이 인스턴스 타입, 힐링도 인스턴스 타입임
		else
		{
			// 피해 계산은 피해를 입는 대상유닛측의 함수 출력
			// 다만, 발사 속도에 맞춰 출력 시킴
			if (Ownerstat.AttackSpeed < Ownerstat.AttackDuration)
			{
				OwnerUnit->Demagecall(targetOwner->GetStat(), targetOwner);
				Ownerstat.AttackDuration = 0.f;

				OwnerUnit->PlayShootSound();
			}
		}
	}


	// 타겟정보가 비었으면 수색 상태로 변경
	if (ptarget.empty())
	{
		GetStateMachine()->ChangeState(L"TraceState");
	}
}

void ShootState::Exit()
{
}