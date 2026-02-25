#include "pch.h"
#include "CAirBullet.h"

#include "CAnimator.h"
#include "CLogMgr.h"
#include "CUnit.h"

CAirBullet::CAirBullet()
	:m_Hit(false)
	, m_EffectAnimOn(false)
{
	// 기초 설정
	m_bulletTex1 = CAssetMgr::Get()->LoadTexture(L"nullbullet1", L"texture\\effect\\spc0_6.png");
	m_bulletTex2 = CAssetMgr::Get()->LoadTexture(L"nullbullet1", L"texture\\effect\\spc0_6.png");
	m_bulletTex3 = CAssetMgr::Get()->LoadTexture(L"nullbullet1", L"texture\\effect\\spc0_6.png");
	m_bulletTex4 = CAssetMgr::Get()->LoadTexture(L"nullbullet1", L"texture\\effect\\spc0_6.png");

	m_Texture = m_bulletTex1;

	SetDuration(0.5f);
	SetSpeed(1000.f);
	SetScale(45.f, 45.f);

	// 충돌체 설정
	m_Body->SetOffset(0.f, 10.f);
	m_Body->SetScale(45.f, 45.f);
	m_Body->ForceDeactive();

	// 애니메이션 설정
	m_EffectAnim = AddComponent(new CAnimator);

	tAnimDesc desc = {};
	desc.AnimName = L"AirSupport_Bullet";
	desc.FPS = 20;
	desc.FrmCount = 1;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"AirBulletAnim", L"texture\\effect\\effect_AirBullet.png");
	desc.SliceSize = Vec2(24.f, 38.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_EffectAnim->CreateAnimation(desc);

	desc.AnimName = L"AirSupport_Bullet_Effect";
	desc.FrmCount = 6;
	desc.SliceSize = Vec2(24.f, 38.f);
	desc.StartLeftTop = Vec2(24.f, 0.f);
	m_EffectAnim->CreateAnimation(desc);

	m_EffectAnim->Play(L"AirSupport_Bullet", false);
}

CAirBullet::~CAirBullet()
{
}

void CAirBullet::Tick()
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
	if (GetPos().x >= m_TargetPos.x && GetPos().y >= m_TargetPos.y && !m_EffectAnimOn)
	{
		SetPos(m_TargetPos);

		m_EffectAnim->Play(L"AirSupport_Bullet_Effect", false);
		m_Body->Activate();
		m_Hit = true;
		m_EffectAnimOn = true;
	}
}

void CAirBullet::Render()
{
	m_EffectAnim->Render();
}

void CAirBullet::Demagecall(CUnit* _type)
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

void CAirBullet::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
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