#include "pch.h"
#include "CLevel_Stage.h"

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
#include "CTextUI.h"
#include "CPlatform.h"
#include "CMiniMap.h"

#include "CRepair.h"
#include "CAirSupport.h"

#include "resource.h"

CLevel_Stage::CLevel_Stage()
	: pBunker(nullptr)
	, eBunker(nullptr)
	, m_LineLegth(1300.f)
	, m_MapCol(2)
	, m_MapIndx(0)
	, m_CameraLook(640.f, 820.f)
	, m_FUNDUpgradeCost(500)
	, m_CoolUpgradeCost(500)
	, m_CoolCount(1)
	, m_FUNDCount(1)
	, m_WinLose(WIN_LOSE::NONE)

{
}

CLevel_Stage::~CLevel_Stage()
{
}

void CLevel_Stage::Init()
{
	// »ç¿îµå
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"BattleBGM", L"sound\\stage_bg.wav");
	CLevelMgr::Get()->SetGameBGM(pBGM);
	CLevelMgr::Get()->PlayGameBGM(false);
	
	pBGM = CAssetMgr::Get()->LoadSound(L"govoice", L"sound\\stage_go.wav");
	pBGM->Play();

	// ½Ã°£ÀÌ ¸ØÃçÀÕÀ» °¡´É¼ºÀÌ ÀÖÀ¸´Ï ÇØÁ¦
	CTimeMgr::Get()->SetTimeStop(false);
	// ÄğÅ¸ÀÓ Àü¿ë ÅØ½ºÃÄ°¡Á®¿À±â
	m_CooltimeTex = CUnitMgr::Get()->GetColltimeTex();
	// ½Â¸®/ÆĞ¹è»óÅÂ ÃÊ±âÈ­
	m_WinLose = WIN_LOSE::NONE;

	// ±âÅ¸ ÅØ½ºÃÄ ¼³Á¤
	m_WinTex = CAssetMgr::Get()->LoadTexture(L"WinTexture", L"texture\\stage\\resultBg1_1.png");
	m_LoseTex = CAssetMgr::Get()->LoadTexture(L"LoseTexture", L"texture\\stage\\resultBg1_0.png");
	m_VSTex = CAssetMgr::Get()->LoadTexture(L"VsTexture", L"texture\\stage\\vs.png");

	// Map Object ¹èÄ¡
	m_BackMap = new CBackMap;
	m_BackMap->SetPlatCol(m_MapCol);
	m_BackMap->SetIndx(m_MapIndx);
	AddObject(m_BackMap, LAYER_TYPE::BACKGROUND);

	m_BackETC = new CBackETC;
	m_BackETC->SetPlatCol(m_MapCol);
	m_BackETC->SetIndx(m_MapIndx);
	AddObject(m_BackETC, LAYER_TYPE::TILE);

	m_PlatMap = new CPlatMap;
	m_PlatMap->SetPlatCol(m_MapCol);
	m_PlatMap->SetIndx(m_MapIndx);
	m_PlatMap->SetPos(0.f, 700.f);
	AddObject(m_PlatMap, LAYER_TYPE::PLATMAP);

	// Bunker(Left)
	pBunker = new LeftBunker;
	pBunker->SetName(L"LeftBunker");
	Vec2 bunkerscale = pBunker->GetScale();
	m_pBunkerPos = Vec2(pBunker->GetScale().x / 2.f, (m_PlatMap->GetScale().y / 2.f) + m_PlatMap->GetPos().y - 28.f);
	pBunker->SetPos(m_pBunkerPos);

	AddObject(pBunker, LAYER_TYPE::PLAYER_BASE);


	// Bunker(Right)
	eBunker = new RightBunker;
	eBunker->SetName(L"RightBunker");
	bunkerscale = eBunker->GetScale();
	m_eBunkerPos = Vec2((m_LineLegth - eBunker->GetScale().x / 2.f), (m_PlatMap->GetScale().y / 2.f) + m_PlatMap->GetPos().y - 28.f);
	eBunker->SetPos(m_eBunkerPos);

	AddObject(eBunker, LAYER_TYPE::ENEMY_BASE);


	// ÆĞ³Î À§Ä¡
	Vec2 vResol = CEngine::Get()->GetResolution();
	pPanel = new CHoldUI;
	pPanel->SetName(L"Origin Panel");
	pPanel->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"UnitPanel", L"texture\\stage\\unitSlot0.png"));
	pPanel->SetPos(Vec2(0.f, vResol.y - 144.f));
	pPanel->SetScale(vResol.x, 144.f);
	AddObject(pPanel, LAYER_TYPE::UI);

	m_FUNDUpgrade = new CBtnUI;
	m_FUNDUpgrade->SetName(L"Butten1 Panel");
	m_FUNDUpgrade->SetScale(Vec2(95.f, 95.f));
	m_FUNDUpgrade->SetPos(Vec2(1030.f, 10.f));
	m_FUNDUpgrade->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"UpgradePanel1", L"texture\\stage\\procKey10.png"));
	m_FUNDUpgrade->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"UpgradePanel2", L"texture\\stage\\procKey11.png"));
	m_FUNDUpgrade->SetOffImg(CAssetMgr::Get()->LoadTexture(L"UpgradePanel2", L"texture\\stage\\procKey11.png"));
	m_FUNDUpgrade->SetDelegate(this, (BaseFunc)&CLevel_Stage::UpgradeFUND);
	pPanel->AddChildUI(m_FUNDUpgrade);

	m_CooltimeUpgrade = new CBtnUI;
	m_CooltimeUpgrade->SetName(L"Butten2 Panel");
	m_CooltimeUpgrade->SetScale(Vec2(95.f, 95.f));
	m_CooltimeUpgrade->SetPos(Vec2(1155.f, 10.f));
	m_CooltimeUpgrade->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"CoolPanel1", L"texture\\stage\\procKey20.png"));
	m_CooltimeUpgrade->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"CoolPanel2", L"texture\\stage\\procKey21.png"));
	m_CooltimeUpgrade->SetOffImg(CAssetMgr::Get()->LoadTexture(L"CoolPanel2", L"texture\\stage\\procKey21.png"));
	m_CooltimeUpgrade->SetDelegate(this, (BaseFunc)&CLevel_Stage::UpgradeCool);
	pPanel->AddChildUI(m_CooltimeUpgrade);

	m_Changemenu = new CBtnUI;
	m_Changemenu->SetName(L"Change Panel");
	m_Changemenu->SetScale(Vec2(440.f, 54.f));
	//m_Changemenu->SetPos(Vec2(450.f, 400.f));
	m_Changemenu->SetPos(Vec2(-100.f, -100.f));
	m_Changemenu->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"Panel1", L"texture\\stage\\resultKey1_0.png"));
	m_Changemenu->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"Panel2", L"texture\\stage\\resultKey1_1.png"));
	m_Changemenu->SetOffImg(CAssetMgr::Get()->LoadTexture(L"Panel2", L"texture\\stage\\resultKey1_1.png"));
	m_Changemenu->SetDelegate(this, (BaseFunc)&CLevel_Stage::Changestartmenu);
	AddObject(m_Changemenu, LAYER_TYPE::UI);

	// Á¤Áö UI
	m_StopUI = new CBtnUI;
	m_StopUI->SetName(L"Pause Panel");
	m_StopUI->SetScale(Vec2(66.f, 68.f));
	m_StopUI->SetPos(Vec2(1150.f, 40.f));
	m_StopUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"pausePanel1", L"texture\\stage\\procKey00.png"));
	m_StopUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"pausePanel2", L"texture\\stage\\procKey01.png"));
	m_StopUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"pausePanel1", L"texture\\stage\\procKey00.png"));
	m_StopUI->SetDelegate(this, (BaseFunc)&CLevel_Stage::StopGame);
	AddObject(m_StopUI, LAYER_TYPE::UI);

	m_PauseUI = nullptr;

	// ¼ÒÈ¯µÉ À¯´Öµé ´ã±â
	SetSummonUnitUI();

	// ½ºÅ³Á¤º¸ ÃÊ±âÈ­
	SetSkillUI();

	// ¹Ì´Ï¸Ê »ı¼º
	m_MiniMap = new CMiniMap;
	m_MiniMap->SetPos(800.f, 50.f);
	AddObject(m_MiniMap, LAYER_TYPE::MINIMAP);

	// Ä«¸Ş¶ó ¼¼ÆÃ
	CCamera::Get()->SetCameraLRLock(false);
	CCamera::Get()->SetCameraUPDOWNLock(true);
	CCamera::Get()->SetCameraStageMode(true);
	CCamera::Get()->SetCameraStatus(COMMANDALLOPEN);


	// À¯´Ö Ãæµ¹ ÁöÁ¤
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::ENEMY_UNIT, (UINT)LAYER_TYPE::PLAYER_UNIT, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_UNIT_PROJECTILE, (UINT)LAYER_TYPE::ENEMY_UNIT, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::ENEMY_UNIT_PROJECTILE, (UINT)LAYER_TYPE::PLAYER_UNIT, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_UNIT, (UINT)LAYER_TYPE::ENEMY_BASE, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::ENEMY_UNIT, (UINT)LAYER_TYPE::PLAYER_BASE, true);




}

void CLevel_Stage::Exit()
{

	// Ä«¸Ş¶ó »óÅÂ ¼ÂÆÃ
	CCamera::Get()->SetCameraStatus(COMMANDALLCLOSE);
	CCamera::Get()->SetCameraLRLock(false);
	CCamera::Get()->SetCameraUPDOWNLock(false);
	CCamera::Get()->SetCameraStageMode(false);

	// ¿ÀºêÁ§Æ® »èÁ¦
	DeleteAllObjects();

	// À¯´Ö UIÁÖ¼Ò »èÁ¦
	m_UnitUI.clear();
	// ½ºÅ³ UIÁÖ¼Ò »èÁ¦
	m_SkillUI.clear();
	// Kill UIÁÖ¼Ò »èÁ¦
	m_vecKillUI.clear();
	// pause¹öÆ° nullptr
	m_PauseUI = nullptr;

	// À¯´Ö ÄğÅ¸ÀÓ º¹±¸
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();
	for (int i = 0; i < UnitIndx.size(); ++i)
	{
		Unit_Stat& pStat = UnitIndx[i]->GetStat();
		pStat.Cooltime = pStat.CooltimeOrgin;
		pStat.CooltimeDuration = 0.f;

	}
	// Àû À¯´Ö ÄğÅ¸ÀÓ º¹±¸
	vector<CUnit*>& EUnitIndx = CUnitMgr::Get()->GetESummonIndx();
	for (int i = 0; i < EUnitIndx.size(); ++i)
	{
		Unit_Stat& pStat = EUnitIndx[i]->GetStat();
		pStat.Cooltime = pStat.CooltimeOrgin;
		pStat.CooltimeDuration = 0.f;
	}
	// ½ºÅ³ ÄğÅ¸ÀÓ º¹±¸
	CUnitMgr::Get()->GetSkill(SKILL_NAME::REPAIR)->GetStat().CooltimeDuration = 0.f;
	CUnitMgr::Get()->GetSkill(SKILL_NAME::AIRSTRIKE)->GetStat().CooltimeDuration = 0.f;
	CUnitMgr::Get()->GetSkill(SKILL_NAME::PRECISIONBOOM)->GetStat().CooltimeDuration = 0.f;

	// Àû À¯´Ö ¸ñ·Ï »èÁ¦
	CUnitMgr::Get()->ClearESummonIndx();

	// ¾÷±×·¹ÀÌµå »óÅÂ º¹±¸
	m_FUNDUpgradeCost = 500;
	m_CoolUpgradeCost = 500;
	m_CoolCount = 1;
	m_FUNDCount = 1;

	// Æ½ÀÌ ¸ØÃçÀÖÀ» ¼ö ÀÖÀ¸´Ï ´Ù½Ã Æ½ ÁøÇà
	CTimeMgr::Get()->SetTimeStop(false);

}

void CLevel_Stage::Tick()
{
	if (CCamera::Get()->GetCameraStat() & DOORALLOPEN)
	{
		CCamera::Get()->SetDoorOn(false);
	}

	// ½½¸®, ÆĞ¹è È®ÀÎ
	if (0.f >= pBunker->GetBaseStat().HP && m_WinLose == WIN_LOSE::NONE)
	{
		CTimeMgr::Get()->SetTimeStop(true);
		m_WinLose = WIN_LOSE::LOSE;
		CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Lose1", L"sound\\lose_1.wav");
		pBGM->Play();
		pBGM = CAssetMgr::Get()->LoadSound(L"Lose2", L"sound\\lose_2.wav");
		pBGM->Play();

		CLevelMgr::Get()->StopGameBGM(false);

		pPanel->ActiveControl(false);
		m_StopUI->ActiveControl(false);
		m_Changemenu->SetPos(Vec2(800.f, 400.f));
		m_Changemenu->Tick();
		KillUI();

	}
	else if (0.f >= eBunker->GetBaseStat().HP && m_WinLose == WIN_LOSE::NONE)
	{
		CTimeMgr::Get()->SetTimeStop(true);
		m_WinLose = WIN_LOSE::WIN;
		CSound* pBGM = CAssetMgr::Get()->LoadSound(L"win1", L"sound\\win_1.wav");
		pBGM->Play();
		pBGM = CAssetMgr::Get()->LoadSound(L"win2", L"sound\\win_2.wav");
		pBGM->Play();

		CLevelMgr::Get()->StopGameBGM(false);

		pPanel->ActiveControl(false);
		m_StopUI->ActiveControl(false);
		m_Changemenu->SetPos(Vec2(800.f, 400.f));
		m_Changemenu->Tick();
		KillUI();
	}
	else
	{
		if (m_WinLose != WIN_LOSE::NONE)
		{
			m_Changemenu->Tick();
		}
		else
		{
		}
	}


	if (!(CTimeMgr::Get()->IsTimeStop()))
	{
		CLevel::Tick();
		// UI¿¡¼­ ÄğÅ¸ÀÓ¿¡ µû¶ó or ÄÚ½ºÆ®¿¡ µû¶ó È°¼º, ºñÈ°¼ºÈ­
		UIOnOffjudge();
	}
	else
	{
		if(m_PauseUI !=nullptr)
			m_PauseUI->Tick();
	}
}

void CLevel_Stage::FinalTick()
{
	if (!(CTimeMgr::Get()->IsTimeStop()))
	{
		CLevel::FinalTick();
	}
	else
	{
		if (m_PauseUI != nullptr)
			m_PauseUI->FinalTick();
	}
}

void CLevel_Stage::Render()
{
	CLevel::Render();

	//==================================================================
	// vsÅØ½ºÃÄ ¹èÄ¡
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(Åõ¸í) ~ 255(ºÒÅõ¸í)
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)625, (int)5
		, m_VSTex->GetWidth(), m_VSTex->GetHeight()
		, m_VSTex->GetDC()
		, 0, 0
		, m_VSTex->GetWidth(), m_VSTex->GetHeight()
		, blend);


	//==================================================================
	// FUND ÅØ½ºÆ® ¹èÄ¡
	int& HaveFUND = pBunker->GetHaveFUND();
	int& MaxFUND = pBunker->GetMaxFUND();

	wstring number = std::to_wstring(HaveFUND);
	wstring number2 = std::to_wstring(MaxFUND);
	{
		wstring str = L"FUND   " + number + L" / " + number2;
		SELECT_FONT(BackDC, FONT_TYPE::HEAVYFONT)
		SetTextColor(BackDC, RGB(255, 255, 0));
		SetTextAlign(BackDC, TA_CENTER);
		SetBkMode(BackDC, TRANSPARENT);
		TextOut(BackDC, (int)1100, (int)590, str.c_str(), str.length());
	}

	// Upgrade
	SELECT_FONT(BackDC, FONT_TYPE::DEFALT)
	{
		SetTextColor(BackDC, RGB(255, 255, 255));
		number = std::to_wstring(m_FUNDUpgradeCost);
		TextOut(BackDC, (int)m_FUNDUpgrade->GetFinalPos().x + 50, (int)m_FUNDUpgrade->GetFinalPos().y + 100, number.c_str(), number.length());

		number = std::to_wstring(m_CoolUpgradeCost);
		TextOut(BackDC, (int)m_CooltimeUpgrade->GetFinalPos().x + 50, (int)m_CooltimeUpgrade->GetFinalPos().y + 100, number.c_str(), number.length());
	}


	//==================================================================
	// ÄÚ½ºÆ®¿¡ µû¸¥ ¼ıÀÚ ÅØ½ºÆ®¹èÄ¡, ÄğÅ¸ÀÓÀÌ¸é ÄğÅ¸ÀÓ ÅØ½ºÃÄ ¹èÄ¡
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();
	if (m_CostTextPos == Vec2(0.f,0.f))
	{
		m_CostTextPos = m_UnitUI[0]->GetFinalPos();
	}
	for (int i = 0; i < m_UnitUI.size(); ++i)
	{
		wstring FUNDtext = std::to_wstring(UnitIndx[i]->GetStat().FUND);
		TextOut(BackDC, (int)m_CostTextPos.x + (125.f *i) + 52, (int)m_CostTextPos.y + 102, FUNDtext.c_str(), FUNDtext.length());
		// ÄğÅ¸ÀÓ¿¡ µû¸¥ ÄğÅ¸ÀÓ ÅØ½ºÃÄ ·£´õ¸µ
		if (!m_UnitUI[i]->GetBtnstat())
		{
			float rate = (float)UnitIndx[i]->GetStat().CooltimeDuration / (float)UnitIndx[i]->GetStat().Cooltime;
			Vec2 vScale = m_UnitUI[i]->GetScale() * Vec2(1.f, rate);
			Vec2 vPos = m_UnitUI[i]->GetFinalPos() + (m_UnitUI[i]->GetScale() * Vec2(1.f, rate));

			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 100; // 0(Åõ¸í) ~ 255(ºÒÅõ¸í)
			blend.AlphaFormat = 0;


			AlphaBlend(BackDC, (int)m_UnitUI[i]->GetFinalPos().x, (int)m_UnitUI[i]->GetFinalPos().y
				, vScale.x, vScale.y
				, m_CooltimeTex->GetDC()
				, 0, 0
				, m_CooltimeTex->GetWidth(), m_CooltimeTex->GetHeight()
				, blend);
		}
	}

	// ½ºÅ³ ÄğÅ¸ÀÓ ÅØ½ºÃÄ ¹èÄ¡
	for (int i = 0; i < m_SkillUI.size(); ++i)
	{
		if (!m_SkillUI[i]->GetBtnstat())
		{
			CSkill* pSkill = m_SkillIndx[i];
			float rate = (float)pSkill->GetStat().CooltimeDuration / (float)pSkill->GetStat().Cooltime;
			Vec2 vScale = m_SkillUI[i]->GetScale() * Vec2(1.f, rate);
			Vec2 vPos = m_SkillUI[i]->GetFinalPos() + (m_SkillUI[i]->GetScale() * Vec2(1.f, rate));

			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 100; // 0(Åõ¸í) ~ 255(ºÒÅõ¸í)
			blend.AlphaFormat = 0;


			AlphaBlend(BackDC, (int)m_SkillUI[i]->GetFinalPos().x, (int)m_SkillUI[i]->GetFinalPos().y
				, vScale.x, vScale.y
				, m_CooltimeTex->GetDC()
				, 0, 0
				, m_CooltimeTex->GetWidth(), m_CooltimeTex->GetHeight()
				, blend);
		}
	}

	// ½Ã°£ÀÌ ¸Ø­Ÿ´Ù¸é °ËÀº È­¸é¶ç¿ò
	if (CTimeMgr::Get()->IsTimeStop())
	{
		blend.SourceConstantAlpha = 100; // 0(Åõ¸í) ~ 255(ºÒÅõ¸í)
		blend.AlphaFormat = 0;

		CTexture* m_FilterTex = CCamera::Get()->GetFilterEffect();
		AlphaBlend(BackDC, 0, 0
			, m_FilterTex->GetWidth()
			, m_FilterTex->GetHeight()
			, m_FilterTex->GetDC()
			, 0, 0
			, m_FilterTex->GetWidth()
			, m_FilterTex->GetHeight()
			, blend);
	}


	//==================================================================
	// ½Â¸®, ÆĞ¹è ÅØ½ºÃÄ
	if (m_WinLose == WIN_LOSE::WIN)
	{
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)750, (int)205
			, m_WinTex->GetWidth(), m_WinTex->GetHeight()
			, m_WinTex->GetDC()
			, 0, 0
			, m_WinTex->GetWidth(), m_WinTex->GetHeight()
			, blend);
	}
	else if (m_WinLose == WIN_LOSE::LOSE)
	{
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)750, (int)205
			, m_LoseTex->GetWidth(), m_LoseTex->GetHeight()
			, m_LoseTex->GetDC()
			, 0, 0
			, m_LoseTex->GetWidth(), m_LoseTex->GetHeight()
			, blend);
	}



	//==================================================================
	// ÇØ´ç ·£´õ¸µÀº µû·Î °ü¸®
	if (m_WinLose != WIN_LOSE::NONE)
	{
		// µ¹¾Æ°¡±â ¹öÆ° ·£´õ¸µ
		m_Changemenu->Render();

		// Å³ Ä«¿îÅÍ ·£´õ¸µ
		vector<CUnit*> EIndx = CUnitMgr::Get()->GetESummonIndx();
		for (int i = 0; i < m_vecKillUI.size(); ++i)
		{
			m_vecKillUI[i]->Render();
			int Count = CUnitMgr::Get()->GetEKillCount(EIndx[i]->GetEUnitName());

			wstring number2 = L"KILL : " + std::to_wstring(Count);
			TextOut(BackDC, (int)m_vecKillUI[i]->GetFinalPos().x + 120, (int)m_vecKillUI[i]->GetFinalPos().y + 30.f, number2.c_str(), number2.length());
		}
	}

	if (nullptr != m_PauseUI)
	{
		m_PauseUI->Render();
	}
}

void CLevel_Stage::SetSummonUnitUI()
{
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();
	Vec2	PUIPos = Vec2(20.f, 10.f);
	for (int i = 0; i < UnitIndx.size(); ++i)
	{
		CDragUI* newUI = new CDragUI;

		newUI->SetScale(Vec2(107.f, 92.f));
		newUI->SetPos(PUIPos.x + (i * 125.f), PUIPos.y);
		newUI->SetNormalImg(UnitIndx[i]->GetIconTexture1(), UnitIndx[i]->GetIconLPos(), UnitIndx[i]->GetIconSize());
		newUI->SetPressedImg(UnitIndx[i]->GetRangeTexture(), Vec2(UnitIndx[i]->GetBulletStart().x, UnitIndx[i]->GetBulletStart().y -UnitIndx[i]->GetStat().Range.y / 2.f), 100, UnitIndx[i]->GetStat().Range);
		newUI->SetOffImg(UnitIndx[i]->GetIconTexture2(), UnitIndx[i]->GetIconLPos(), UnitIndx[i]->GetIconSize());
		UnitIndx[i]->PlayAnimation(L"Stend", true);
		newUI->SetAnimation(UnitIndx[i]->GetAinmator()->Clone());
		newUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_Stage::SommonUnit, (DWORD_PTR)UnitIndx[i]->GetPUnitName());

		m_UnitUI.push_back(newUI);
		pPanel->AddChildUI(newUI);
	}

}

void CLevel_Stage::SetSkillUI()
{
	// Repair½ºÅ³ µî·Ï
	CSkill* pRepair = CUnitMgr::Get()->GetSkill(SKILL_NAME::REPAIR);
	m_SkillIndx.push_back(pRepair);

	m_RepairUI = new CBtnUI;
	m_RepairUI->SetName(L"Skill1 Panel");
	m_RepairUI->SetScale(Vec2(80.f, 80.f));
	m_RepairUI->SetPos(Vec2(30.f, 50.f));
	m_RepairUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_RepairUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_RepairUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon2", L"texture\\stage\\SkillIcon01.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_RepairUI->SetDelegate(pRepair, (BaseFunc)&CRepair::ActiveSkill);
	AddObject(m_RepairUI, LAYER_TYPE::UI);
	m_SkillUI.push_back(m_RepairUI);

	m_RepaircostTex = new CTextUI;
	m_RepaircostTex->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillCostPanel", L"texture\\stage\\SkillCostIcon.png"));
	m_RepaircostTex->SetPos(Vec2(m_RepairUI->GetPos().x, m_RepairUI->GetPos().y + 70.f));
	m_RepaircostTex->SetScale(80.f, 24.f);
	wstring SkillFUND = std::to_wstring(pRepair->GetStat().FUND);
	m_RepaircostTex->SetText(SkillFUND);
	m_RepaircostTex->SetTextOffset(Vec2(0.f, -10.f));
	AddObject(m_RepaircostTex, LAYER_TYPE::UI);


	// µÎ¹øÂ° ½ºÅ³ ÆĞ³Î µî·Ï
	pRepair = CUnitMgr::Get()->GetSkill(SKILL_NAME::AIRSTRIKE);
	m_SkillIndx.push_back(pRepair);
	
	m_AirStrikeUI = new CDragUI;
	m_AirStrikeUI->SetScale(Vec2(80.f, 80.f));
	m_AirStrikeUI->SetPos(30.f + 100.f, 50.f);
	m_AirStrikeUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_AirStrikeUI->SetPressedImg(pRepair->GetRangeTexture(), Vec2(0.f,0.f), Vec2(pRepair->GetRangeTexture()->GetWidth(), pRepair->GetRangeTexture()->GetHeight()), Vec2(-1500.f,-40.f), 100, Vec2(3000.f, 80.f));
	m_AirStrikeUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon2", L"texture\\stage\\SkillIcon01.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_AirStrikeUI->SetAnimation(pRepair->GetDomiAnim()->Clone());
	m_AirStrikeUI->SetDelegate(pRepair, (BaseFunc)&CAirSupport::ActiveSkill);
	AddObject(m_AirStrikeUI, LAYER_TYPE::UI);
	m_SkillUI.push_back(m_AirStrikeUI);
	
	m_AirStrikecostTex = new CTextUI;
	m_AirStrikecostTex->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillCostPanel", L"texture\\stage\\SkillCostIcon.png"));
	m_AirStrikecostTex->SetPos(Vec2(m_AirStrikeUI->GetPos().x, m_AirStrikeUI->GetPos().y + 70.f));
	m_AirStrikecostTex->SetScale(80.f, 24.f);
	SkillFUND = std::to_wstring(pRepair->GetStat().FUND);
	m_AirStrikecostTex->SetText(SkillFUND);
	m_AirStrikecostTex->SetTextOffset(Vec2(0.f, -10.f));
	AddObject(m_AirStrikecostTex, LAYER_TYPE::UI);

	// ¼¼¹ø¤Š ½ºÅ³ ÆĞ³Î µî·Ï
	pRepair = CUnitMgr::Get()->GetSkill(SKILL_NAME::PRECISIONBOOM);
	m_SkillIndx.push_back(pRepair);

	m_PrecisionBoomUI = new CDragUI;
	m_PrecisionBoomUI->SetScale(Vec2(80.f, 80.f));
	m_PrecisionBoomUI->SetPos(30.f + 200.f, 50.f);
	m_PrecisionBoomUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_PrecisionBoomUI->SetPressedImg(pRepair->GetRangeTexture(), Vec2(0.f, 0.f), Vec2(pRepair->GetRangeTexture()->GetWidth(), pRepair->GetRangeTexture()->GetHeight()), -pRepair->GetScale()/2.f, 100, pRepair->GetScale());
	m_PrecisionBoomUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"SkillIcon2", L"texture\\stage\\SkillIcon01.png"), pRepair->GetIconLPos(), pRepair->GetIconSize());
	m_PrecisionBoomUI->SetAnimation(pRepair->GetDomiAnim()->Clone());
	m_PrecisionBoomUI->SetDelegate(pRepair, (BaseFunc)&CAirSupport::ActiveSkill);
	AddObject(m_PrecisionBoomUI, LAYER_TYPE::UI);
	m_SkillUI.push_back(m_PrecisionBoomUI);

	m_PrecisionBoomTex = new CTextUI;
	m_PrecisionBoomTex->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"SkillCostPanel", L"texture\\stage\\SkillCostIcon.png"));
	m_PrecisionBoomTex->SetPos(Vec2(m_PrecisionBoomUI->GetPos().x, m_PrecisionBoomUI->GetPos().y + 70.f));
	m_PrecisionBoomTex->SetScale(80.f, 24.f);
	SkillFUND = std::to_wstring(pRepair->GetStat().FUND);
	m_PrecisionBoomTex->SetText(SkillFUND);
	m_PrecisionBoomTex->SetTextOffset(Vec2(0.f, -10.f));
	AddObject(m_PrecisionBoomTex, LAYER_TYPE::UI);
}

void CLevel_Stage::UIOnOffjudge()
{
	// À¯´Ö ¸Å´ÏÀú¿¡¼­ À¯´Ö Á¤º¸¹è¿­À» °¡Á®¿Â´Ù.
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();

	// ÄÚ½ºÆ®¸¦ È®ÀÎÇÑ´Ù.
	int HaveFUND = pBunker->GetHaveFUND();

	// À¯´ÖUI¹è¿­À» ¼øÂ÷ÀûÀ¸·Î È®ÀÎ, UIÀÇ ¹è¿­Àº À¯´Ö Á¤º¸¹è¿­ ¼ø¼­¿Í µ¿ÀÏ

	for (int i = 0; i < m_UnitUI.size(); ++i)
	{
		Unit_Stat& fStat = UnitIndx[i]->GetStat();
		// ÄğÅ¸ÀÓÀÌ ¿Ï·áµÇÁö ¾Ê°Å³ª ÄÚ½ºÆ®°¡ ºÎÁ·ÇÒ ½Ã
		if (0.f < fStat.CooltimeDuration || HaveFUND < fStat.FUND)
		{
			m_UnitUI[i]->ActiveControl(false);
			m_UnitUI[i]->SetBtnstat(false);
		}
		else
		{
			m_UnitUI[i]->ActiveControl(true);
			m_UnitUI[i]->SetBtnstat(true);
		}
	}

	// UpgradeÆĞ³Î ÄÚ½ºÆ® È®ÀÎ
	if (HaveFUND < m_FUNDUpgradeCost)
	{
		m_FUNDUpgrade->ActiveControl(false);
		m_FUNDUpgrade->SetBtnstat(false);
	}
	else
	{
		m_FUNDUpgrade->ActiveControl(true);
		m_FUNDUpgrade->SetBtnstat(true);
	}
	if (HaveFUND < m_CoolUpgradeCost)
	{
		m_CooltimeUpgrade->ActiveControl(false);
		m_CooltimeUpgrade->SetBtnstat(false);
	}
	else
	{
		m_CooltimeUpgrade->ActiveControl(true);
		m_CooltimeUpgrade->SetBtnstat(true);
	}

	// SkillÆĞ³Î ÄÚ½ºÆ® È®ÀÎ
	// Repair
	for (int i = 0; i < m_SkillUI.size(); ++i)
	{
		Unit_Stat& SkilStat = CUnitMgr::Get()->GetSkill((SKILL_NAME)i)->GetStat();

		if (0.f < SkilStat.CooltimeDuration || HaveFUND < SkilStat.FUND)
		{
			m_SkillUI[i]->ActiveControl(false);
			m_SkillUI[i]->SetBtnstat(false);
		}
		else
		{
			m_SkillUI[i]->ActiveControl(true);
			m_SkillUI[i]->SetBtnstat(true);
		}
	}


}

void CLevel_Stage::Changestartmenu()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(LEVEL_TYPE::START);
}

void CLevel_Stage::KillUI()
{
	vector<CUnit*>& CurESummonIndx = CUnitMgr::Get()->GetESummonIndx();
	int col = 1;
	int row = 1;
	for (int i = 0; i < CurESummonIndx.size(); ++i)
	{
		// ÇàÀº 3°³ ±îÁö Ç¥±â
		if (row == 4)
		{
			++col;
			row = 1;
		}

		// È¦µå UI¹èÄ¡
		CHoldUI* KillPanel = new CHoldUI;
		KillPanel->SetName(L"Kill Panel");
		KillPanel->SetNormalImg(CurESummonIndx[i]->GetIconTexture1(), CurESummonIndx[i]->GetIconLPos(), CurESummonIndx[i]->GetIconSize());
		KillPanel->SetPos(Vec2(100.f + 170.f * row, 120.f * col));
		KillPanel->SetScale(80.f, 80.f);
		AddObject(KillPanel, LAYER_TYPE::UI);
		m_vecKillUI.push_back(KillPanel);

		// ´ÙÀ½ ÇàÀ¸·Î
		++row;
	}


}



void CLevel_Stage::UpgradeFUND()
{
	int& HaveFUND = pBunker->GetHaveFUND();
	int& MaxFUND = pBunker->GetMaxFUND();
	if (HaveFUND >= m_FUNDUpgradeCost)
	{
		HaveFUND -= m_FUNDUpgradeCost;
		MaxFUND += m_FUNDUpgradeCost;
		m_FUNDUpgradeCost = static_cast<int>(m_FUNDUpgradeCost * 1.5f);
		++m_FUNDCount;
	}
}

void CLevel_Stage::UpgradeCool()
{
	// À¯´Ö ¸Å´ÏÀú¿¡¼­ À¯´Ö Á¤º¸¹è¿­À» °¡Á®¿Â´Ù.
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();

	int& HaveFUND = pBunker->GetHaveFUND();
	int& MaxFUND = pBunker->GetMaxFUND();
	if (HaveFUND >= m_CoolUpgradeCost)
	{
		// À¯´ÖÀÇ Äğ´Ù¿î ½ºÅİÀ» 5%°¨¼Ò½ÃÅ²´Ù
		for (int i = 0; i < UnitIndx.size(); ++i)
		{
			Unit_Stat& stat = UnitIndx[i]->GetStat();

			// ÄğÅ¸ÀÓÀÌ ÁÙ¾îµç ¸¸Å­ ÇöÀç ÄğÅ¸ÀÓµµ Â÷°¨µÈ´Ù.
			if (0.f < stat.CooltimeDuration)
			{
				stat.CooltimeDuration -= (stat.Cooltime - stat.CooltimeOrgin * ((float)1 - (float)m_CoolCount * 0.1f));
			}

			stat.Cooltime = stat.CooltimeOrgin * ((float)1 - (float)m_CoolCount * 0.1f);
		}

		HaveFUND -= m_CoolUpgradeCost;
		++m_CoolCount;
		m_CoolUpgradeCost = static_cast<int>(m_CoolUpgradeCost * 1.5f);
	}
}


void CLevel_Stage::SommonUnit(PLAYER_UNIT_NAME _Unit)
{
	Vec2 MouseRealPos = CCamera::Get()->GetRealPos(CKeyMgr::Get()->GetMousePos());

	CUnit* SUnit = CUnitMgr::Get()->GetPlayerUnit(_Unit);

	// ¿ø·¡¶ó¸é ÄğÅ¸ÀÓÀÌ 0À¸·Î ¶³¾îÁö°í È£ÃâÇØ¾ß¸¸ ÇÑ´Ù.
	if (SUnit->GetStat().CooltimeDuration > 0.f)
	{
		LOG(LOG_LEVEL::BUG, L"À¯´ÖÀÇ ÄğÅ¸ÀÓÀÌ ³¡³ª±â Àü¿¡ ¼ÒÈ¯µÇ°í ÀÖ½À´Ï´Ù.")
	}
	// ÄÚ½ºÆ®¸¦ ÃÊ°úÇÔ¿¡µµ Ãâ·Â ÇßÀ» °æ¿ì
	int& HaveFUND = pBunker->GetHaveFUND();
	if (HaveFUND < SUnit->GetStat().FUND)
	{
		LOG(LOG_LEVEL::BUG, L"À¯´ÖÀÇ ÄÚ½ºÆ®°¡ ºÎÁ·ÇÑµ¥ ¼ÒÈ¯µÇ°í ÀÖ½À´Ï´Ù.")
			return;
	}

	// °Å¸® ÃøÁ¤
	Vec2 vScale = SUnit->GetScale();
	Vec2 vBodyPos = SUnit->GetBodyPos();
	Vec2 fieldPos = m_Platform->GetPos();
	Vec2 fieldScale = m_Platform->GetScale();

	Vec2 vDiff = fieldPos - MouseRealPos - vBodyPos;

	vDiff.x = abs(vDiff.x);
	vDiff.y = abs(vDiff.y);
	float ScaleX = fieldScale.x / 2.f + vScale.x / 2.f;

	float UnderY = vDiff.y - vScale.y / 2.f;
	float UpY = vDiff.y + vScale.y / 2.f;

	// ÀüÀå À§Ä¡¸¦ ¹ş¾î³­°÷ÀÌ ¾Æ´ÔÀ» È®ÀÎÇÏ¸é ¼ÒÈ¯
	if (fieldScale.y / 2.f >= UnderY && fieldScale.y / 2.f >= UpY )
	{
		SUnit->GetStat().CooltimeDuration = SUnit->GetStat().Cooltime;
		pBunker->GetHaveFUND() -= SUnit->GetStat().FUND;
		SUnit = SUnit->Clone();
		SUnit->SetPos(pBunker->GetBodyPos().x - pBunker->GetScale().x/2.f, MouseRealPos.y);
		AddObject(SUnit, LAYER_TYPE::PLAYER_UNIT);
	}

	

}

void CLevel_Stage::StopGame()
{
	if (m_PauseUI == nullptr)
	{
		CTimeMgr::Get()->SetTimeStop(true);
		CLevelMgr::Get()->StopGameBGM(false);
		pPanel->ActiveControl(false);
		m_StopUI->ActiveControl(false);

		m_PauseUI = new CHoldUI;
		m_PauseUI->SetName(L"Origin Panel");
		m_PauseUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"PausePanel", L"texture\\stage\\pauseBg.png"));
		m_PauseUI->SetPos(Vec2(300.f, 200.f));
		m_PauseUI->SetScale(670, 286.f);
		AddObject(m_PauseUI, LAYER_TYPE::UI);

		m_RestartUI = new CBtnUI;
		m_RestartUI->SetName(L"Butten1 Panel");
		m_RestartUI->SetScale(Vec2(440.f, 54.f));
		m_RestartUI->SetPos(Vec2(100.f, 70.f));
		m_RestartUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn1", L"texture\\stage\\pauseKey1_00.png"));
		m_RestartUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn2", L"texture\\stage\\pauseKey1_01.png"));
		m_RestartUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn2", L"texture\\stage\\pauseKey1_01.png"));
		m_RestartUI->SetDelegate(this, (BaseFunc)&CLevel_Stage::RestartGame);
		m_PauseUI->AddChildUI(m_RestartUI);

		m_ChangeMenuUI = new CBtnUI;
		m_ChangeMenuUI->SetName(L"Butten2 Panel");
		m_ChangeMenuUI->SetScale(Vec2(440.f, 54.f));
		m_ChangeMenuUI->SetPos(Vec2(100.f, 180.f));
		m_ChangeMenuUI->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn3", L"texture\\stage\\pauseKey1_10.png"));
		m_ChangeMenuUI->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn4", L"texture\\stage\\pauseKey1_11.png"));
		m_ChangeMenuUI->SetOffImg(CAssetMgr::Get()->LoadTexture(L"pauseBtn4", L"texture\\stage\\pauseKey1_11.png"));
		m_ChangeMenuUI->SetDelegate(this, (BaseFunc)&CLevel_Stage::Changestartmenu);
		m_PauseUI->AddChildUI(m_ChangeMenuUI);
	}
}

void CLevel_Stage::RestartGame()
{
	CTimeMgr::Get()->SetTimeStop(false);
	CLevelMgr::Get()->PlayGameBGM(true);
	pPanel->ActiveControl(true);
	m_StopUI->ActiveControl(true);

	DeleteObject(m_PauseUI);
	m_PauseUI = nullptr;
}




Vec2 CLevel_Stage::GetStageLength()
{
	return Vec2(pBunker->GetScale().x + eBunker->GetScale().x + m_LineLegth, m_Platform->GetScale().y);
}

Vec2 CLevel_Stage::GetLTopPos()
{
	float x = pBunker->GetBodyPos().x - pBunker->GetBodyScale().x / 2.f;
	float y = pBunker->GetBodyPos().y - pBunker->GetBodyScale().y / 2.f;
	return Vec2(x,y);
}























// ========================================================================

void CLevel_Stage::LoadMap(const wstring& _RelativePath)
{
	// ÆÄÀÏ °æ·Î ¹®ÀÚ¿­
	wstring strFilePath = CPathMgr::Get()->GetContentPath();
	strFilePath += _RelativePath;


	if (wcslen(strFilePath.c_str()) != 0)
	{

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, strFilePath.c_str(), L"r");

		wchar_t szRead[255] = {};
		while (true)
		{
			if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
				break;

			wstring str = szRead;

			if (str == L"[MAP_SELECT]")
			{
				wchar_t mapread[255] = {};
				while (true)
				{
					fwscanf_s(pFile, L"%s", mapread, 255);
					if (!wcscmp(L"END", mapread))
					{
						str = mapread;
						break;
					}
					if (!wcscmp(L"Map_Indx", mapread))
					{
						fwscanf_s(pFile, L"%d", &m_MapIndx);
						m_BackMap->SetIndx(m_MapIndx);
						m_BackETC->SetIndx(m_MapIndx);
						m_PlatMap->SetIndx(m_MapIndx);
					}

					else if (!wcscmp(L"Map_Col", mapread))
					{
						fwscanf_s(pFile, L"%d", &m_MapCol);
						m_BackMap->SetPlatCol(m_MapCol);
						m_BackETC->SetPlatCol(m_MapCol);
						m_PlatMap->SetPlatCol(m_MapCol);
					}

					else if (!wcscmp(L"Map_Legth", mapread))
					{
						fwscanf_s(pFile, L"%f", &m_LineLegth);
					}

					else if (!wcscmp(L"Map_Pos", mapread))
					{
						fwscanf_s(pFile, L"%f %f %f %f %f %f", &m_MapPosBack.x, &m_MapPosBack.y, &m_MapPosBackE.x, &m_MapPosBackE.y, &m_MapPlat.x, &m_MapPlat.y);
					}
				}
			}

			else if (str == L"[BASE_POS]")
			{
				wchar_t Buffread[255] = {};

				while (true)
				{
					if (EOF == fwscanf_s(pFile, L"%s", Buffread, 255))
					{
						break;
					}
					if (!wcscmp(L"BASE_LEFT", Buffread))
					{
						fwscanf_s(pFile, L"%f %f", &m_pBunkerPos.x, &m_pBunkerPos.y);
						pBunker->SetPos(m_pBunkerPos);
					}

					else if (!wcscmp(L"BASE_RIGHT", Buffread))
					{
						fwscanf_s(pFile, L"%f %f", &m_eBunkerPos.x, &m_eBunkerPos.y);
						eBunker->SetPos(m_eBunkerPos);
					}

				}
			}
		}

		fclose(pFile);
	}




	CUIMgr::Get()->SetPreventFrame(1);

}