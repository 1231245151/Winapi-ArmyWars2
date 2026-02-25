#include "pch.h"
#include "CUnit.h"

#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CCollider.h"
#include "CSearchRange.h"


CUnit::CUnit()
	:m_Animator(nullptr)
	, m_Body(nullptr)
	, m_State(nullptr)
	, m_AtMoveType(UNIT_ATST::MOVE)
	, m_Death(false)
	, m_BodyOffset(0.f, 10.f)
	, m_BulletOffset(10.f, 10.f)
	, m_PUnitName(PLAYER_UNIT_NAME::NONE)
	, m_EUnitName(ENEMY_UNIT_NAME::NONE)

{
	// 몸위치 설정
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"Body Collider");

	// 범위 설정
	m_Range = AddComponent(new CSearchRange(&m_TargetObj));
	m_Range->SetName(L"Search Range");
	m_Range->SetOffset((GetScale().x / 2.f) + (GetStat().Range.x / 2.f), 10.f);
	m_Range->SetScale(GetStat().Range);

	// state 설정
	m_State = AddComponent(new CStateMachine);
	m_State->AddState(L"TraceState", new TraceState);
	m_State->AddState(L"ShootState", new ShootState);
	m_State->AddState(L"DeathState", new DeathState);
	m_State->ChangeState(L"TraceState");

	// Anim설정
	m_Animator = AddComponent(new CAnimator);


	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"pIconUnit1", L"texture\\player\\UnitIcon00.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconScale = Vec2(80.f, 80.f);


	m_BulletStart = Vec2(GetPos().x, GetPos().y + 10.f);
}

CUnit::CUnit(const CUnit& _Other)
	:CObj(_Other)
	, m_Stat(_Other.m_Stat)
	, m_AtMoveType(_Other.m_AtMoveType)
	, m_ShootInst(_Other.m_ShootInst)
	, m_MoveStop(false)
	, m_Death(false)
	, m_PUnitName(_Other.m_PUnitName)
	, m_EUnitName(_Other.m_EUnitName)
	, m_BulletStart(_Other.m_BulletStart)
	, m_BodyOffset(_Other.m_BodyOffset)
	, m_RangeTexture(_Other.m_RangeTexture)
	, m_BulletOffset(_Other.m_BulletOffset)
{
	m_Body = (CCollider*)GetComponentByName(L"Body Collider");

	m_Range = GetComponent<CSearchRange>();
	m_Range->SetVector(&m_TargetObj);

	m_State = GetComponent<CStateMachine>();

	m_Animator = GetComponent<CAnimator>();
}

CUnit::~CUnit()
{

}

void CUnit::Tick()
{
	// 체력이 0이면 사망 상태로 전환
	if (m_Stat.HP <= 0.f && false == m_Death)
	{
		m_State->ChangeState(L"DeathState");
		m_Death = true;
	}

	// 사격 딜레이 계산
	m_Stat.AttackDuration += DT;
}

void CUnit::Render()
{
	m_Animator->Render();

	// HPUI 랜더링
	HPUI();


	// 사거리 랜더링 테스트
	//if (nullptr != m_RangeTexture)
	//{
	//	Vec2 vFinalPos = GetRenderPos();
	//	Vec2 vScale = GetScale();
	//
	//	BLENDFUNCTION blend = {};
	//	blend.BlendOp = AC_SRC_OVER;
	//	blend.BlendFlags = 0;
	//	blend.SourceConstantAlpha = 100; // 0(투명) ~ 255(불투명)
	//	blend.AlphaFormat = AC_SRC_ALPHA;
	//
	//	AlphaBlend(BackDC, (int)vFinalPos.x + m_BulletStart.x, (int)vFinalPos.y- m_Stat.Range.y/2.f + m_BulletStart.y
	//		, m_Stat.Range.x, m_Stat.Range.y
	//		, m_RangeTexture->GetDC()
	//		, 0, 0
	//		, m_RangeTexture->GetWidth(), m_RangeTexture->GetHeight()
	//		, blend);
	//}
}


void CUnit::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	m_Animator->Play(_Name, _Repeat);
}

void CUnit::ShootBullet(Vec2 _Dir)
{
	m_BulletStart = GetPos() + m_BulletOffset;
	if (m_Stat.AttackSpeed < m_Stat.AttackDuration)
	{
		CBullet* bullet = new CSmallBullet;
		bullet->SetPos(m_BulletStart);
		bullet->SetDir(_Dir);
		bullet->SetDemage(GetStat().HDemage, GetStat().MDemage);

		CreateObject(bullet, LAYER_TYPE::PLAYER_UNIT_PROJECTILE);
		m_Stat.AttackDuration = 0.f;
		PlayShootSound();

		// 이펙트 추가
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CShootBullet* neweffect = new CShootBullet;
		neweffect->SetOwner(this);
		neweffect->SetOffset(m_BulletOffset);
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
	}

}

void CUnit::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	// 공격 대상 타입이 인간
	if (UNIT_TYPE::HUMEN == _EnemyStat.Type)
	{
		int Demage = m_Stat.HDemage - _EnemyStat.Armor;

		if (0 < Demage)
			_EnemyStat.HP -= Demage;
	}
	// 공격 대상 타입이 기계
	else if(UNIT_TYPE::MACHINE == _EnemyStat.Type)
	{
		int Demage = m_Stat.MDemage - _EnemyStat.Armor;

		if (0 < Demage)
			_EnemyStat.HP -= Demage;
	}
	// 공격 대상 타입이 그 외일 경우
	else
	{
		if (_EnemyStat.HDemage >= _EnemyStat.MDemage)
		{
			int Demage = m_Stat.HDemage - _EnemyStat.Armor;

			if (0 < Demage)
				_EnemyStat.HP -= Demage;
		}
		else if (_EnemyStat.HDemage < _EnemyStat.MDemage)
		{
			int Demage = _EnemyStat.MDemage - m_Stat.Armor;

			if (0 < Demage)
				_EnemyStat.HP -= Demage;
		}
	}
}

void CUnit::HPUI()
{
	Vec2 RenderPos = GetRenderPos() + Vec2(-15.f, 30.f);
	float HPrate = 0;
	if (0 < m_Stat.HP)
	{
		HPrate = (float)m_Stat.HP / (float)m_Stat.MaxHP;
	}
	else
	{
		HPrate = 0.f;
	}


	SELECT_BRUSH(BackDC, BRUSH_TYPE::RED);
	Vec2 vScale = Vec2(30.f,5.f) * Vec2(HPrate,1.f);

	Rectangle(BackDC, (int)RenderPos.x, (int)RenderPos.y
		, (int)RenderPos.x + (int)vScale.x, (int)RenderPos.y + (int)vScale.y);
}





void CUnit::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if ( (L"Body Collider" == _OwnCollider->GetName() && L"Body Collider" == _OtherCollider->GetName()) || ( L"Body Collider" == _OwnCollider->GetName() && L"BunkerBody Collider" == _OtherCollider->GetName() ) )
	{
		m_MoveStop = true;

		if(L"DeathState" != m_State->PresentStateName())
		PlayAnimation(L"Stend", true);
	}

}

void CUnit::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if ((L"Body Collider" == _OwnCollider->GetName() && L"Body Collider" == _OtherCollider->GetName()) || (L"Body Collider" == _OwnCollider->GetName() && L"BunkerBody Collider" == _OtherCollider->GetName()))
	{
		if (false == m_MoveStop)
		{
			m_MoveStop = true;

			if (L"DeathState" != m_State->PresentStateName())
				PlayAnimation(L"Stend", true);
		}
	}
}

void CUnit::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if ( (L"Body Collider" == _OwnCollider->GetName() && L"Body Collider" == _OtherCollider->GetName()) || (L"Body Collider" == _OwnCollider->GetName() && L"BunkerBody Collider" == _OtherCollider->GetName() ) )
	{
		m_MoveStop = false;

		if (L"DeathState" != m_State->PresentStateName())
		PlayAnimation(L"Move", true);
	}
}