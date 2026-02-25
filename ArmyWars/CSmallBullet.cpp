#include "pch.h"
#include "CSmallBullet.h"

#include "CHitBullet.h"
#include "CLevel.h"
#include "CLevelMgr.h"

CSmallBullet::CSmallBullet()
{

	m_bulletTex1 = CAssetMgr::Get()->LoadTexture(L"Smallbullet1", L"texture\\effect\\fire0_3.png");
	m_bulletTex2 = CAssetMgr::Get()->LoadTexture(L"Smallbullet2", L"texture\\effect\\fire0_5.png");
	m_bulletTex3 = CAssetMgr::Get()->LoadTexture(L"Smallbullet3", L"texture\\effect\\fire0_7.png");
	m_bulletTex4 = CAssetMgr::Get()->LoadTexture(L"Smallbullet4", L"texture\\effect\\fire0_9.png");

	m_Texture = CAssetMgr::Get()->LoadTexture(L"Smallbullet1", L"texture\\effect\\fire0_3.png");

	SetDuration(10.f);
	SetSpeed(700.f);
	SetScale(20.f, 5.f);

	// 충돌체 설정
	m_Body->SetOffset(0.f, 0.f);
	m_Body->SetScale(10.f, 10.f);
}

CSmallBullet::~CSmallBullet()
{
}

void CSmallBullet::Demagecall(CUnit* _type)
{
	CBullet::Demagecall(_type);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CHitBullet* neweffect = new CHitBullet;
	CObj* Obj = (CObj*)_type;
	neweffect->SetOwner(Obj);
	neweffect->SetOffset(Vec2(0.f, 10.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}