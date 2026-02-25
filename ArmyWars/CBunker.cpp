#include "pch.h"
#include "CBunker.h"

#include "CEngine.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CHitBullet.h"

#include "CCollider.h"
#include "CSearchRange.h"
#include "CStateMachine.h"
#include "BaseTraceState.h"
#include "EnemySommonState.h"
#include "CTimeMgr.h"

CBunker::CBunker()
	:m_BaseStat()
	, m_MaxFund(1000)
	, m_HaveFund(500)
	, m_FUNDDuration(5.f)
	, m_FUNDRate(0.f)
	, m_FUNDHeal(100)
	, m_BodyOffset(0,28.f)
	, m_SummonState(nullptr)
{
	// 몸위치 설정
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"BunkerBody Collider");
	m_Body->SetScale(144.f, 360.f);
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_Range = AddComponent(new CSearchRange(&m_TargetObj));
	m_Range->SetName(L"BunkerRange");
	m_Range->SetScale(200.f, 360.f);
	m_Range->SetOffset(180.f, 28.f);

	// state 설정
	m_State = AddComponent(new CStateMachine);
	m_State->SetName(L"TraceState");
	m_State->AddState(L"BaseTraceState", new BaseTraceState);
	m_State->ChangeState(L"BaseTraceState");

	m_BaseStat.MaxHP = 5000;
	m_BaseStat.HP = 5000;
	m_BaseStat.Range = Vec2(144.f, 360.f);
	m_BaseStat.HDemage = 30;
	m_BaseStat.MDemage = 30;
	m_BaseStat.AttackSpeed = 0.4;
	m_BaseStat.AttackDuration = 0;
	m_BaseStat.Type = UNIT_TYPE::BASE;

}

CBunker::~CBunker()
{
}

void CBunker::Tick()
{
	// 사격 딜레이 계산
	m_BaseStat.AttackDuration += DT;

	// 코스트 회복
	m_FUNDRate += DT;
	if (m_FUNDDuration < m_FUNDRate)
	{
		FlusFUND(m_FUNDHeal);
		m_FUNDRate -= m_FUNDDuration;
	}

}

void CBunker::Render()
{
	UINT width = GetTexture()->GetWidth();
	UINT height = GetTexture()->GetHeight();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;

	Vec2 RenderPos = GetRenderPos();
	Vec2 ScalePos = GetScale();

	AlphaBlend(BackDC, (int)RenderPos.x - (int)ScalePos.x / 2, (int)RenderPos.y - (int)ScalePos.y / 2
		, (int)ScalePos.x, (int)ScalePos.y
		, GetTexture()->GetDC()
		, 0, 0
		, GetTexture()->GetWidth(), GetTexture()->GetHeight()
		, blend);


	HPUI();
}

void CBunker::FlusFUND(int _FUND)
{
	if (m_MaxFund - m_HaveFund < _FUND)
		m_HaveFund = m_MaxFund;
	else
		m_HaveFund += _FUND;
}

Vec2 CBunker::GetBodyPos()
{
	return (GetPos() + m_BodyOffset);
}

Vec2 CBunker::GetBodyScale()
{
	return m_Body->GetScale();
}

void CBunker::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	if (m_BaseStat.HDemage >= m_BaseStat.MDemage)
	{
		int Demage = m_BaseStat.HDemage;

		if (0 < Demage)
			_EnemyStat.HP -= Demage;
	}
	else if (m_BaseStat.HDemage < m_BaseStat.MDemage)
	{
		int Demage = m_BaseStat.MDemage;

		if (0 < Demage)
			_EnemyStat.HP -= Demage;
	}

	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"Bullect", L"sound\\shoot_bullet.wav");
	pBullet->Play();

	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CHitBullet* neweffect = new CHitBullet;
	neweffect->SetOwner(_Other);
	neweffect->SetOffset(Vec2(0.f, -10.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void CBunker::HPUI()
{
	Vec2 RenderPos = Vec2(0.f, 0.f);
	float HPrate = 0;
	if (0 < m_BaseStat.HP)
	{
		HPrate = (float)m_BaseStat.HP / (float)m_BaseStat.MaxHP;
	}
	else
	{
		HPrate = 0.f;
	}


	SELECT_BRUSH(BackDC, BRUSH_TYPE::RED);
	Vec2 vScale = Vec2(600.f, 20.f) * Vec2(HPrate, 1.f);

	Rectangle(BackDC, (int)RenderPos.x, (int)RenderPos.y
		, (int)RenderPos.x + (int)vScale.x, (int)RenderPos.y + (int)vScale.y);
}

void CBunker::SetSummonState()
{
	// 적이라면 소환 스테이트 추가 설정
	m_SummonState = AddComponent(new CStateMachine);
	m_SummonState->SetName(L"SummonState");
	m_SummonState->AddState(L"SummonState", new EnemySommonState);
	m_SummonState->ChangeState(L"SummonState");
}

void CBunker::DeleteSummonState()
{
	if (nullptr != m_SummonState)
	{
		delete m_SummonState;
		m_SummonState = nullptr;
	}
}


void CBunker::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if ( (L"BunkerRange Collider" == _OwnCollider->GetName()) && (L"Body Collider" == _OtherCollider->GetName()) )
	{
		m_TargetObj.push_back(_OtherObj);
	}
}

void CBunker::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if ( (L"BunkerRange Collider" == _OwnCollider->GetName()) && (L"Body Collider" == _OtherCollider->GetName()) ) 
	{

	}
}