#include "pch.h"
#include "CLevel_Stage01.h"

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

#include "CUnitMgr.h"
#include "CUnit.h"
#include "PSolider.h"
#include "ESolider.h"
#include "PSWAT.h"
#include "ESWAT.h"
#include "PMedic.h"
#include "PHumvee.h"

#include "CUIMgr.h"
#include "CBtnUI.h"
#include "CHoldUI.h"
#include "CPlatform.h"

#include "resource.h"

CLevel_Stage01::CLevel_Stage01()
{
}

CLevel_Stage01::~CLevel_Stage01()
{
}

void CLevel_Stage01::Init()
{
	CLevel_Stage::Init();

	// 맵 정보 로드
	LoadMap(L"map\\stage_01.map");

	// =====================================
	// Load후 적용
	// =====================================

	// 적 소환 정보 설정
		// 적 소환 정보 설정
	CUnitMgr::Get()->ClearESummonIndx();
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SOLIDER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SWAT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::MEDIC);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SNIPER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SCOUT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HEAVYSOLIDR);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::DESTROYER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HUMVEE);
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

	eBunker->GetBaseStat().MaxHP = 2000;
	eBunker->GetBaseStat().HP = 2000;
	eBunker->GetBaseStat().HDemage = 30;
	eBunker->GetBaseStat().MDemage = 30;
}
