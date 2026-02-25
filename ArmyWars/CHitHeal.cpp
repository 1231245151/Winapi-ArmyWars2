#include "pch.h"
#include "CHitHeal.h"

CHitHeal::CHitHeal()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 10;
	desc.FrmCount = 8;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_Heal", L"texture\\effect\\effect_heal.png");
	desc.SliceSize = Vec2(33.7f, 49.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CHitHeal::~CHitHeal()
{
}
