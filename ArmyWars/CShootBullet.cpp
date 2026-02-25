#include "pch.h"
#include "CShootBullet.h"


CShootBullet::CShootBullet()
	:m_Duration(0.1f)
{

	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 5;
	desc.FrmCount = 1;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_bullet", L"texture\\effect\\fireEffect0_0.png");
	desc.SliceSize = Vec2(28.f, 20.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CShootBullet::~CShootBullet()
{
}

void CShootBullet::Tick()
{
	m_Duration -= DT;
	if (m_Duration < 0.f)
	{
		DeleteObject(this);
	}

	if (nullptr != m_Owner)
	{
		SetPos(m_Owner->GetPos() + m_Offset);
	}
}