#include "pch.h"
#include "CShootTank2.h"

CShootTank2::CShootTank2()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 20;
	desc.FrmCount = 6;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_Tank2", L"texture\\effect\\effect_shoottank.png");
	desc.SliceSize = Vec2(84.f, 55.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CShootTank2::~CShootTank2()
{
}
