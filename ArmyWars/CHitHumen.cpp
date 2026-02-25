#include "pch.h"
#include "CHitHumen.h"




CHitHumen::CHitHumen()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 20;
	desc.FrmCount = 8;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_humenblood", L"texture\\effect\\effect_blood.png");
	desc.SliceSize = Vec2(61.f, 39.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CHitHumen::~CHitHumen()
{
}
