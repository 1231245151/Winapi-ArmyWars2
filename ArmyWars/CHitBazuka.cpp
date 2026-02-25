#include "pch.h"
#include "CHitBazuka.h"

CHitBazuka::CHitBazuka()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 15;
	desc.FrmCount = 9;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_boombazuka", L"texture\\effect\\effect_bazukaboom.png");
	desc.SliceSize = Vec2(59.f, 105.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CHitBazuka::~CHitBazuka()
{
}
