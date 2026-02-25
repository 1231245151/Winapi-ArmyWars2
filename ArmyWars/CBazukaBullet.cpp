#include "pch.h"
#include "CBazukaBullet.h"

#include "CHitBazuka.h"
#include "CLevel.h"
#include "CLevelMgr.h"


CBazukaBullet::CBazukaBullet()
{
	m_bulletTex1 = CAssetMgr::Get()->LoadTexture(L"Bazukabullet1", L"texture\\effect\\spc3_4.png");
	m_bulletTex2 = CAssetMgr::Get()->LoadTexture(L"Bazukabullet2", L"texture\\effect\\spc3_3.png");
	m_bulletTex3 = CAssetMgr::Get()->LoadTexture(L"Bazukabullet3", L"texture\\effect\\spc3_2.png");
	m_bulletTex4 = CAssetMgr::Get()->LoadTexture(L"Bazukabullet4", L"texture\\effect\\spc3_1.png");

	m_Texture = CAssetMgr::Get()->LoadTexture(L"Bazukabullet1", L"texture\\effect\\spc3_4.png");

	SetDuration(10.f);
	SetSpeed(200.f);
	SetScale(32.f, 11.f);

	// 충돌체 설정
	m_Body->SetOffset(0.f, 0.f);
	m_Body->SetScale(20.f, 20.f);
}

CBazukaBullet::~CBazukaBullet()
{
}

void CBazukaBullet::Demagecall(CUnit* _type)
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