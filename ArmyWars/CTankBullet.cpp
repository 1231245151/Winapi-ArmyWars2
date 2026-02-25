#include "pch.h"
#include "CTankBullet.h"

#include "CHitBazuka.h"
#include "CLevel.h"
#include "CLevelMgr.h"

CTankBullet::CTankBullet()
{
	m_bulletTex1 = CAssetMgr::Get()->LoadTexture(L"Tankbullet1", L"texture\\effect\\fire1_4.png");
	m_bulletTex2 = CAssetMgr::Get()->LoadTexture(L"Tankbullet2", L"texture\\effect\\fire1_3.png");
	m_bulletTex3 = CAssetMgr::Get()->LoadTexture(L"Tankbullet3", L"texture\\effect\\fire1_2.png");
	m_bulletTex4 = CAssetMgr::Get()->LoadTexture(L"Tankbullet4", L"texture\\effect\\fire1_1.png");

	m_Texture = CAssetMgr::Get()->LoadTexture(L"Tankbullet1", L"texture\\effect\\fire1_4.png");

	SetDuration(10.f);
	SetSpeed(400.f);
	SetScale(39.f, 10.f);

	// 충돌체 설정
	m_Body->SetOffset(0.f, 0.f);
	m_Body->SetScale(30.f, 15.f);
}

CTankBullet::~CTankBullet()
{
}

void CTankBullet::Demagecall(CUnit* _type)
{
	CBullet::Demagecall(_type);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CHitBazuka* neweffect = new CHitBazuka;
	CObj* Obj = (CObj*)_type;
	neweffect->SetOwner(Obj);
	neweffect->SetOffset(Vec2(0.f, -10.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}