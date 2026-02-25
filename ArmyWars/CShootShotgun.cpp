#include "pch.h"
#include "CShootShotgun.h"


CShootShotgun::CShootShotgun()
{
	tAnimDesc desc = {};
	desc.AnimName = L"Effect_shoot";
	desc.FPS = 15;
	desc.FrmCount = 8;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Effect_Shotgun", L"texture\\effect\\effect_shotgun.png");
	desc.SliceSize = Vec2(37.f, 22.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animation->CreateAnimation(desc);

	m_Animation->Play(L"Effect_shoot", false);
}

CShootShotgun::~CShootShotgun()
{
}
