#include "pch.h"
#include "CLargeBoomEffect.h"

CLargeBoomEffect::CLargeBoomEffect()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 8;
	desc.FrmCount = 5;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_boom2", L"texture\\effect\\effect_boom2.png");
	desc.SliceSize = Vec2(159.f, 147.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CLargeBoomEffect::~CLargeBoomEffect()
{
}
