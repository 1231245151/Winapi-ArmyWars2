#include "pch.h"
#include "CHitBullet.h"

CHitBullet::CHitBullet()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 20;
	desc.FrmCount = 9;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_Bullethit", L"texture\\effect\\effect_Hit.png");
	desc.SliceSize = Vec2(61.f, 41.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CHitBullet::~CHitBullet()
{
}
