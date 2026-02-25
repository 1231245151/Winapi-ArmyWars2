#include "pch.h"
#include "CSkill.h"


CSkill::CSkill()
	:m_Duration(3.f)
	, m_DomiAnim(nullptr)
{
	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"SkillIcon2", L"texture\\stage\\SkillIcon01.png");
	m_IconScale = Vec2(80.f, 80.f);

	// Anim설정
	m_Animator = AddComponent(new CAnimator);
}

CSkill::CSkill(const CSkill& _Other)
	:CObj(_Other)
	, m_SkillStat(_Other.m_SkillStat)
	, m_Duration(_Other.m_Duration)
	, m_DomiAnim(_Other.m_DomiAnim)

{
	m_Animator = GetComponent<CAnimator>();
}

CSkill::~CSkill()
{
}

void CSkill::Tick()
{
	// 유지 시간 종료 사 삭제
	m_Duration -= DT;
	if (m_Duration < 0.f)
		DeleteObject(this);
}

void CSkill::Render()
{
	m_Animator->Render();
}