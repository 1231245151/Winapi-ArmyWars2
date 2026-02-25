#include "pch.h"
#include "CLevel_StgSelect.h"

#include "LeftBunker.h"
#include "RightBunker.h"
#include "CMap.h"
#include "CPlatMap.h"
#include "CBackETC.h"
#include "CBackMap.h"

#include "CUnit.h"
#include "CUnitMgr.h"
#include "PSolider.h"
#include "ESolider.h"
#include "PSWAT.h"
#include "ESWAT.h"
#include "PMedic.h"
#include "PHumvee.h"

#include "CUIMgr.h"
#include "CBtnUI.h"
#include "CHoldUI.h"
#include "CDragUI.h"
#include "CPlatform.h"


CLevel_StgSelect::CLevel_StgSelect()
{
}

CLevel_StgSelect::~CLevel_StgSelect()
{
}

void CLevel_StgSelect::Init()
{
	CCamera::Get()->SetCameraStatus(COMMANDALLOPEN);

	m_PrevStage = new CBtnUI;
	m_PrevStage->SetName(L"Prev2 Panel");
	m_PrevStage->SetScale(Vec2(230.f, 54.f));
	m_PrevStage->SetPos(Vec2(100.f, 630.f));
	m_PrevStage->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"PrevLevel1", L"texture\\select\\stageSelectKey1_00.png"));
	m_PrevStage->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"PrevLevel2", L"texture\\select\\stageSelectKey1_01.png"));
	m_PrevStage->SetDelegate(this, (BaseFunc)&CLevel_StgSelect::PrevLevel);
	AddObject(m_PrevStage, LAYER_TYPE::UI);

	m_StageNameTexture = new CHoldUI;
	m_StageNameTexture->SetName(L"Stage Name Panel");
	m_StageNameTexture->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"StageName1", L"texture\\select\\stageSelectTitle1_0.png"));
	m_StageNameTexture->SetPos(Vec2(600.f, 50.f));
	m_StageNameTexture->SetScale(560, 62.f);
	AddObject(m_StageNameTexture, LAYER_TYPE::UI);

	CBtnUI* BtnUI = new CBtnUI;
	BtnUI->SetName(L"Batton1");
	BtnUI->SetScale(Vec2(128.f, 130.f));
	BtnUI->SetPos(Vec2(200.f, 200.f));
	BtnUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Batton0_0", L"texture\\select\\stageSelectSlot00.png"));
	BtnUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Batton0_1", L"texture\\select\\stageSelectSlot01.png"));
	BtnUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_StgSelect::NextLevel, (DWORD_PTR)LEVEL_TYPE::STAGE_TEST);
	AddObject(BtnUI, LAYER_TYPE::UI);

	BtnUI = new CBtnUI;
	BtnUI->SetName(L"Batton2");
	BtnUI->SetScale(Vec2(128.f, 130.f));
	BtnUI->SetPos(Vec2(400.f, 200.f));
	BtnUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Batton1_0", L"texture\\select\\stageSelectSlot10.png"));
	BtnUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Batton1_1", L"texture\\select\\stageSelectSlot11.png"));
	BtnUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_StgSelect::NextLevel, (DWORD_PTR)LEVEL_TYPE::STAGE_01);
	AddObject(BtnUI, LAYER_TYPE::UI);

	BtnUI = new CBtnUI;
	BtnUI->SetName(L"Batton3");
	BtnUI->SetScale(Vec2(128.f, 130.f));
	BtnUI->SetPos(Vec2(600.f, 200.f));
	BtnUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Batton2_0", L"texture\\select\\stageSelectSlot20.png"));
	BtnUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Batton2_1", L"texture\\select\\stageSelectSlot21.png"));
	BtnUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_StgSelect::NextLevel, (DWORD_PTR)LEVEL_TYPE::STAGE_02);
	AddObject(BtnUI, LAYER_TYPE::UI);

	BtnUI = new CBtnUI;
	BtnUI->SetName(L"Batton4");
	BtnUI->SetScale(Vec2(128.f, 130.f));
	BtnUI->SetPos(Vec2(800.f, 200.f));
	BtnUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Batton3_0", L"texture\\select\\stageSelectSlot30.png"));
	BtnUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Batton3_1", L"texture\\select\\stageSelectSlot31.png"));
	BtnUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_StgSelect::NextLevel, (DWORD_PTR)LEVEL_TYPE::STAGE_03);
	AddObject(BtnUI, LAYER_TYPE::UI);

	BtnUI = new CBtnUI;
	BtnUI->SetName(L"Batton5");
	BtnUI->SetScale(Vec2(128.f, 130.f));
	BtnUI->SetPos(Vec2(1000.f, 200.f));
	BtnUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Batton4_0", L"texture\\select\\stageSelectSlot40.png"));
	BtnUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Batton4_1", L"texture\\select\\stageSelectSlot41.png"));
	BtnUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_StgSelect::NextLevel, (DWORD_PTR)LEVEL_TYPE::STAGE_04);
	AddObject(BtnUI, LAYER_TYPE::UI);
}

void CLevel_StgSelect::Exit()
{
	m_StageBtnUI.clear();
}

void CLevel_StgSelect::Tick()
{
	CLevel::Tick();
}

void CLevel_StgSelect::Render()
{
	CLevel::Render();
}


void CLevel_StgSelect::NextLevel(LEVEL_TYPE _Level)
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	CLevelMgr::Get()->SetStageSelectLayer(_Level);
	ChangeLevel(LEVEL_TYPE::UNIT_SELECT);
}

void CLevel_StgSelect::PrevLevel()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(LEVEL_TYPE::START);
}