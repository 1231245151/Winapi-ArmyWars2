#include "pch.h"
#include "CPrecBoom.h"

#include "CAnimator.h"
#include "CLogMgr.h"
#include "CUnit.h"
#include "CLargeBoomEffect.h"

CPrecBoom::CPrecBoom()
	:m_EffectAnim(nullptr)
{
	// 기초 설정
	m_bulletTex1 = CAssetMgr::Get()->LoadTexture(L"nullbullet2", L"texture\\effect\\spc2_5.png");
	m_bulletTex2 = CAssetMgr::Get()->LoadTexture(L"nullbullet2", L"texture\\effect\\spc2_5.png");
	m_bulletTex3 = CAssetMgr::Get()->LoadTexture(L"nullbullet2", L"texture\\effect\\spc2_5.png");
	m_bulletTex4 = CAssetMgr::Get()->LoadTexture(L"nullbullet2", L"texture\\effect\\spc2_5.png");

	m_Texture = m_bulletTex1;

	SetDuration(5.f);
	SetSpeed(800.f);
	SetScale(140.f, 140.f);

	// 충돌체 설정
	m_Body->SetOffset(0.f, 10.f);
	m_Body->SetScale(140.f, 140.f);
	m_Body->ForceDeactive();
}

CPrecBoom::~CPrecBoom()
{
}

void CPrecBoom::Tick()
{
	// 시간 지나면 없어짐
	m_Duration -= DT;

	if (0.f >= m_Duration)
		DeleteObject(this);


	//총알 이동
	if (!m_Hit)
	{
		Vec2 vPos = GetPos();
		vPos += (m_Dir * m_speed) * DT;
		SetPos(vPos);
	}
	// 지정된 위치 이상이면 충돌 활성화, 이펙트 애니메이션
	if (GetPos().x >= m_TargetPos.x && GetPos().y >= m_TargetPos.y && !m_EffectAnimOn && !m_Hit)
	{
		SetPos(m_TargetPos);
		m_Body->Activate();

		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		m_EffectAnim = new CLargeBoomEffect;
		m_EffectAnim->SetPos(GetPos());
		m_EffectAnim->SetOffset(Vec2(0.f, -30.f));
		CurLevel->AddObject(m_EffectAnim, LAYER_TYPE::EFFECT);
		SetDuration(0.5f);

		m_Hit = true;
		m_EffectAnimOn = true;

		CSound* pBullet = CAssetMgr::Get()->LoadSound(L"boomSkill", L"sound\\skill_precboom.wav");
		pBullet->Play();
	}
}

void CPrecBoom::Render()
{
	if (!m_Hit)
		CBullet::Render();
}

void CPrecBoom::Demagecall(CUnit* _type)
{
	Unit_Stat& stat = _type->GetStat();

	if (UNIT_TYPE::HUMEN == stat.Type)
	{
		int demage = m_HDemage;
		if (demage > 0)
			stat.HP -= demage;
	}
	else if (UNIT_TYPE::MACHINE == stat.Type)
	{
		int demage = m_MDemage;
		if (demage > 0)
			stat.HP -= demage;
	}
}

void CPrecBoom::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (L"Body Collider" == _OtherCollider->GetName())
	{
		// 대상은 유닛이어야 함
		// 만일 유닛이 아닌 다른걸 건들면 잘못된 타겟
		CUnit* Target = dynamic_cast<CUnit*>(_OtherObj);
		if (nullptr == Target)
			assert(Target);


		if (Target->GetStat().HP > 0.f)
			Demagecall(Target);
	}
}