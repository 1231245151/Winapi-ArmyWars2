#include "pch.h"
#include "CLevel_Test.h"

#include "CLogMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CTexture.h"
#include "CAssetMgr.h"
#include "CPathMgr.h"

#include "CEngine.h"

#include "LeftBunker.h"
#include "RightBunker.h"
#include "CMap.h"
#include "CPlatMap.h"
#include "CBackETC.h"
#include "CBackMap.h"

#include "CUnit.h"
#include "PSolider.h"
#include "ESolider.h"
#include "PSWAT.h"
#include "ESWAT.h"
#include "EMedic.h"
#include "EHumvee.h"
#include "ESniper.h"
#include "PMedic.h"
#include "PHumvee.h"

#include "CUIMgr.h"
#include "CBtnUI.h"
#include "CHoldUI.h"
#include "CPlatform.h"
#include "CAirSupport.h"

#include "CUnitMgr.h"
#include "resource.h"

CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
}

void CLevel_Test::Init()
{
	CLevel_Stage::Init();

	// 맵 정보 로드
	LoadMap(L"map\\testmap.map");

	// =====================================
	// Load후 적용
	// =====================================

	// 적 소환 정보 설정
	CUnitMgr::Get()->ClearESummonIndx();
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SOLIDER);
	//CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SWAT);
	//CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::MEDIC);
	//CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SCOUT);
	//CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SNIPER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HEAVYSOLIDR);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HUMVEE);
	//CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::DESTROYER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::IFV);
	eBunker->SetSummonState();

   // 전장 위치용
	m_Platform = new CPlatform;
	m_Platform->SetPos((eBunker->GetBodyPos().x - pBunker->GetBodyPos().x) / 2.f + pBunker->GetBodyPos().x, pBunker->GetBodyPos().y);
	m_Platform->SetScale(m_LineLegth, 360.f);
	AddObject(m_Platform, LAYER_TYPE::PLATFORM);

	// 카메라 상태 셋팅
	Vec2 vResolution = CEngine::Get()->GetResolution();
	CCamera::Get()->SetStageCamerMaxPos(m_CameraLook.x, eBunker->GetBodyPos().x - vResolution.x / 2.f + eBunker->GetScale().x / 2.f);



	// 카메라 이동
	CCamera::Get()->SetLookAt(m_CameraLook);




	// ========================================================================================

	// 유닛 테스트
	CUnit* pUnit;
	//pUnit = new PSolider;
	//pUnit->SetPos(pBunker->GetPos() + Vec2(0.f, 120.f));
	//AddObject(pUnit, LAYER_TYPE::PLAYER_UNIT);
	
	//pUnit = new PSolider;
	//pUnit->SetPos(pBunker->GetPos() + Vec2(0.f, -20.f));
	//AddObject(pUnit, LAYER_TYPE::PLAYER_UNIT);

	//pUnit = new PSWAT;
	//pUnit->SetPos(pBunker->GetPos() + Vec2(0.f, 40.f));
	//AddObject(pUnit, LAYER_TYPE::PLAYER_UNIT);
	
	//pUnit = new PMedic;
	//pUnit->SetPos(pBunker->GetPos() + Vec2(0.f, 40.f));
	//AddObject(pUnit, LAYER_TYPE::PLAYER_UNIT);


	//pUnit = new PHumvee;
	//pUnit->SetPos(pBunker->GetPos() + Vec2(0.f, -150.f));
	//AddObject(pUnit, LAYER_TYPE::PLAYER_UNIT);


	//pUnit = new ESolider;
	//pUnit->SetPos(eBunker->GetPos() + Vec2(00.f, 120.f));
	//AddObject(pUnit, LAYER_TYPE::ENEMY_UNIT);
	//
	//pUnit = new ESolider;
	//pUnit->SetPos(eBunker->GetPos() + Vec2(10.f, 0.f));
	//AddObject(pUnit, LAYER_TYPE::ENEMY_UNIT);
	//
	//pUnit = new ESWAT;
	//pUnit->SetPos(eBunker->GetPos() + Vec2(10.f, 140.f));
	//AddObject(pUnit, LAYER_TYPE::ENEMY_UNIT);
	//
	//
	//pUnit = new ESniper;
	//pUnit->SetPos(eBunker->GetPos() + Vec2(10.f, 140.f));
	//AddObject(pUnit, LAYER_TYPE::ENEMY_UNIT);
	//
	//
	//pUnit = new EHumvee;
	//pUnit->SetPos(eBunker->GetPos() + Vec2(10.f, -40.f));
	//AddObject(pUnit, LAYER_TYPE::ENEMY_UNIT);
	
	eBunker->GetBaseStat().MaxHP = 2000;
	eBunker->GetBaseStat().HP = 2000;
	eBunker->GetBaseStat().HDemage = 30;
	eBunker->GetBaseStat().MDemage = 30;
}