#include "pch.h"
#include "CShootBazuka.h"


CShootBazuka::CShootBazuka()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 20;
	desc.FrmCount = 10;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_Bazuka", L"texture\\effect\\effect_bazuka.png");
	desc.SliceSize = Vec2(90.f, 27.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CShootBazuka::~CShootBazuka()
{
}
