#include "pch.h"
#include "CLevel_Stage02.h"

#include "CEngine.h"
#include "CUnitMgr.h"
#include "CPlatform.h"
#include "LeftBunker.h"
#include "RightBunker.h"

CLevel_Stage02::CLevel_Stage02()
{
}

CLevel_Stage02::~CLevel_Stage02()
{
}

void CLevel_Stage02::Init()
{
	CLevel_Stage::Init();

	// 맵 정보 로드
	LoadMap(L"map\\stage_02.map");

	// =====================================
	// Load후 적용
	// =====================================

	// 적 소환 정보 설정
	CUnitMgr::Get()->ClearESummonIndx();
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SOLIDER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SWAT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::SCOUT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HEAVYSOLIDR);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::HUMVEE);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::DESTROYER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::VETERANSOLIDER);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::VETERANSWAT);
	CUnitMgr::Get()->SetESummonIndx(ENEMY_UNIT_NAME::BRADLY);
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
	eBunker->GetBaseStat().HDemage = 35;
	eBunker->GetBaseStat().MDemage = 35;
}