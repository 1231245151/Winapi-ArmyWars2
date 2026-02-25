#include "pch.h"
#include "CLevel_Stage04.h"

#include "CEngine.h"
#include "CUnitMgr.h"
#include "CPlatform.h"
#include "LeftBunker.h"
#include "RightBunker.h"

CLevel_Stage04::CLevel_Stage04()
{
}

CLevel_Stage04::~CLevel_Stage04()
{
}

void CLevel_Stage04::Init()
{
	CLevel_Stage::Init();

	// 맵 정보 로드
	LoadMap(L"map\\stage_05.map");

	// =====================================
	// Load후 적용
	// =====================================

	// 적 소환 정보 설정
	CUnitMgr::Get()->ClearESummonIndx();
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SOLIDER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SWAT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SNIPER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HEAVYSOLIDR);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::VETERANSOLIDER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::DESTROYER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HUMVEE);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::TRUCK);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::BRADLY);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::IFV);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::K2TANK);
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
	eBunker->GetBaseStat().HDemage = 45;
	eBunker->GetBaseStat().MDemage = 45;
}