#include "pch.h"
#include "CEffect.h"


CEffect::CEffect()
	:m_Offset(0.f,0.f)
	,m_Owner(nullptr)
{
	// 애니메이션 등록
	m_Animation = AddComponent<CAnimator>(new CAnimator);
}

CEffect::~CEffect()
{
}

void CEffect::Tick()
{
	// 애니메이션이 종료되면 삭제
	if (m_Animation->IsFinish())
	{
		DeleteObject(this);
	}

	if (nullptr != m_Owner)
	{
		SetPos(m_Owner->GetPos() + m_Offset);
	}
	else
	{
		SetPos(GetPos());
	}
}

void CEffect::Render()
{
	m_Animation->Render();
}