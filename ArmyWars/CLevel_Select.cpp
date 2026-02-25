#include "pch.h"
#include "CLevel_Select.h"

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
#include "CPlatform.h"

CLevel_Select::CLevel_Select()
	: m_Unitpage(0)
	, m_CurPage(0)
{
}

CLevel_Select::~CLevel_Select()
{
}

void CLevel_Select::Init()
{
	CCamera::Get()->SetCameraStatus(COMMANDALLOPEN);

	// Unit 패널 등록
	Vec2 vResol = CEngine::Get()->GetResolution();
	pPanel = new CHoldUI;
	pPanel->SetName(L"Origin Panel");
	pPanel->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"UnitPanel", L"texture\\stage\\unitSlot0.png"));
	pPanel->SetPos(Vec2(0.f, vResol.y - 144.f));
	pPanel->SetScale(vResol.x, 144.f);
	AddObject(pPanel, LAYER_TYPE::UI);

	m_SelectPanel = new CHoldUI;
	m_SelectPanel->SetName(L"Select Panel");
	m_SelectPanel->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"BlackPanel", L"texture\\select\\unitBg.png"));
	m_SelectPanel->SetPos(Vec2(200.f, 10.f));
	m_SelectPanel->SetScale(900, 500.f);
	AddObject(m_SelectPanel, LAYER_TYPE::UI);

	// 버튼 등록
	m_NextStage = new CBtnUI;
	m_NextStage->SetName(L"Next Panel");
	m_NextStage->SetScale(Vec2(400.f, 54.f));
	m_NextStage->SetPos(Vec2(900.f, 550.f));
	m_NextStage->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"NextPanel1", L"texture\\select\\unitKey1_10.png"));
	m_NextStage->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"NextPanel2", L"texture\\select\\unitKey1_11.png"));
	m_NextStage->SetOffImg(CAssetMgr::Get()->LoadTexture(L"NextPanel1", L"texture\\select\\unitKey1_10.png"));
	m_NextStage->SetDelegate(this, (BaseFunc)&CLevel_Select::NextLevel);
	AddObject(m_NextStage, LAYER_TYPE::UI);

	m_PrevStage = new CBtnUI;
	m_PrevStage->SetName(L"Prev Panel");
	m_PrevStage->SetScale(Vec2(400.f, 54.f));
	m_PrevStage->SetPos(Vec2(-10.f, 550.f));
	m_PrevStage->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"PrevrPanel1", L"texture\\select\\unitKey1_00.png"));
	m_PrevStage->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"PrevPanel2", L"texture\\select\\unitKey1_01.png"));
	m_PrevStage->SetOffImg(CAssetMgr::Get()->LoadTexture(L"PrevPanel1", L"texture\\select\\unitKey1_00.png"));
	m_PrevStage->SetDelegate(this, (BaseFunc)&CLevel_Select::PrevLevel);
	AddObject(m_PrevStage, LAYER_TYPE::UI);


	m_NextUnit = new CBtnUI;
	m_NextUnit->SetName(L"Next Panel");
	m_NextUnit->SetScale(Vec2(64.f, 260.f));
	m_NextUnit->SetPos(Vec2(1150.f, 200.f));
	m_NextUnit->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"NextUnitPanel1", L"texture\\select\\stageSelectArrow01.png"));
	m_NextUnit->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"NextUnitPanel2", L"texture\\select\\stageSelectArrow11.png"));
	m_NextUnit->SetOffImg(CAssetMgr::Get()->LoadTexture(L"NextUnitPanel1", L"texture\\select\\stageSelectArrow01.png"));
	m_NextUnit->SetDelegate(this, (BaseFunc)&CLevel_Select::NextUnitUI);
	AddObject(m_NextUnit, LAYER_TYPE::UI);

	m_PrevUnit = new CBtnUI;
	m_PrevUnit->SetName(L"Prev Panel");
	m_PrevUnit->SetScale(Vec2(64.f, 260.f));
	m_PrevUnit->SetPos(Vec2(80.f, 200.f));
	m_PrevUnit->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"PrevUnitPanel1", L"texture\\select\\stageSelectArrow0.png"));
	m_PrevUnit->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"PrevUnitPanel2", L"texture\\select\\stageSelectArrow1.png"));
	m_PrevUnit->SetOffImg(CAssetMgr::Get()->LoadTexture(L"PrevUnitPanel1", L"texture\\select\\stageSelectArrow0.png"));
	m_PrevUnit->SetDelegate(this, (BaseFunc)&CLevel_Select::PrevUnitUI);
	AddObject(m_PrevUnit, LAYER_TYPE::UI);


	SetSummonUnitUI();

	SetSelectUnitUI();
}

void CLevel_Select::Exit()
{
	m_UnitUI.clear();
	m_SelectUnitUI.clear();
	m_Unitpage = 0;
}

void CLevel_Select::Tick()
{
	CLevel::Tick();
}

void CLevel_Select::Render()
{
	CLevel::Render();

	TextRender();
}

void CLevel_Select::ClickSelectUI(PLAYER_UNIT_NAME _Name)
{
	// 유닛 정보 가져오기
	CUnit* CurUnit = CUnitMgr::Get()->GetPUnit(_Name);

	// 새로운 UI등장
	m_SelectPanelPlus = new CHoldUI;
	m_SelectPanelPlus->SetName(L"Select Panel");
	m_SelectPanelPlus->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"BlackIndxPanel", L"texture\\select\\wndBg.png"));
	m_SelectPanelPlus->SetPos(Vec2(200.f, 10.f));
	m_SelectPanelPlus->SetScale(550, 606.f);
	AddObject(m_SelectPanelPlus, LAYER_TYPE::UI);


	Vec2	PUIPos = Vec2(20.f, 10.f);
	CBtnUI* newUI = new CBtnUI;
	CBtnUI* newchildUI = new CBtnUI;

	newchildUI->SetScale(Vec2(80.f, 80.f));
	newchildUI->SetPos(PUIPos.x, PUIPos.y);
	newchildUI->SetNormalImg(CurUnit->GetIconTexture1(), Vec2(0.f, 320.f), CurUnit->GetIconSize());
	newchildUI->SetPressedImg(CurUnit->GetIconTexture2(), Vec2(0.f, 320.f), CurUnit->GetIconSize());
	newchildUI->SetOffImg(CurUnit->GetIconTexture2(), Vec2(0.f, 320.f), CurUnit->GetIconSize());
	m_SelectPanelPlus->AddChildUI(newchildUI);

	newUI->SetScale(Vec2(80.f, 80.f));
	newUI->SetPos(0.f, 0.f);
	newUI->SetNormalImg(CurUnit->GetIconTexture1(), CurUnit->GetIconLPos(), CurUnit->GetIconSize());
	newUI->SetPressedImg(CurUnit->GetIconTexture2(), CurUnit->GetIconLPos(), CurUnit->GetIconSize());
	newUI->SetOffImg(CurUnit->GetIconTexture2(), CurUnit->GetIconLPos(), CurUnit->GetIconSize());
	newUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_Select::ClickSelect, (DWORD_PTR)CurUnit->GetPUnitName());
	newchildUI->AddChildUI(newUI);


	// 스텟 텍스트 띄우기
	//SetTextAlign(BackDC, TA_CENTER);
	//SetBkMode(BackDC, TRANSPARENT);
	//SELECT_FONT(BackDC, FONT_TYPE::MEDIUMFONT)
	//{
	//	wstring FUNDtext = std::to_wstring(UnitIndx[i]->GetStat().FUND);
	//	std::wstringstream wss;
	//	wss << std::fixed << std::setprecision(2) << CurUnitStat.CooltimeDuration;
	//	std::wstring str = wss.str();
	//	TextOutW(BackDC, (int)m_CostTextPos.x + (125.f * i) + 52, (int)m_CostTextPos.y + 102, FUNDtext.c_str(), FUNDtext.length());
	//}

	// 그외 UI들을 건드리지 못하게함
	m_SelectPanel->ActiveControl(false);
	pPanel->ActiveControl(false);
}

void CLevel_Select::ClickSelect(PLAYER_UNIT_NAME _Name)
{
	if (8 <= m_UnitUI.size())
	{
		LOG(LOG_LEVEL::LOG,L"꽉차서 넣지 않음")
		return;
	}


	// 목록 가져오기
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();

	// 목록에 소환 될 유닛 추가
	CUnitMgr::Get()->SetPSummonIndx(_Name);

	// 해당 SelectUI 비활성화
	m_SelectUnitUI[(int)_Name]->GetChildUI()[0]->SetBtnstat(false);

	// UI생성, m_UnitUI에 추가
	Vec2	PUIPos = Vec2(20.f, 10.f);
	int VecUIsize = m_UnitUI.size();
	CBtnUI* newUI = new CBtnUI;

	newUI->SetScale(Vec2(107.f, 92.f));
	newUI->SetPos(PUIPos.x + ((float)(VecUIsize) * 125.f), PUIPos.y);
	newUI->SetNormalImg(UnitIndx[VecUIsize]->GetIconTexture1(), UnitIndx[VecUIsize]->GetIconLPos(), UnitIndx[VecUIsize]->GetIconSize());
	newUI->SetPressedImg(UnitIndx[VecUIsize]->GetIconTexture1(), UnitIndx[VecUIsize]->GetIconLPos(), UnitIndx[VecUIsize]->GetIconSize());
	newUI->SetOffImg(UnitIndx[VecUIsize]->GetIconTexture2(), UnitIndx[VecUIsize]->GetIconLPos(), UnitIndx[VecUIsize]->GetIconSize());
	newUI->SetDelegate2(this, (BaseFunc_Param2)&CLevel_Select::ClickSummon, (DWORD_PTR)newUI, (DWORD_PTR)UnitIndx[VecUIsize]->GetPUnitName());

	m_UnitUI.push_back(newUI);
	pPanel->AddChildUI(newUI);

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
}

void CLevel_Select::ClickSummon(CUI* _UI, PLAYER_UNIT_NAME _Name)
{
	// 목록 가져오기
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();

	// SelectUI 활성화
	m_SelectUnitUI[(int)_Name]->GetChildUI()[0]->SetBtnstat(true);

	// m_UnitUI UI에서 제거, 해당 UI제거
	vector<CUnit*>::iterator iter = UnitIndx.begin();
	vector<CBtnUI*>::iterator iterUI = m_UnitUI.begin();
	for (int i = 0; iter != UnitIndx.end(); ++i)
	{

		if ((*iter)->GetPUnitName() == _Name)
		{
			DeleteObject(*iterUI);
			pPanel->DeleteChildUI(*iterUI);
			m_UnitUI.erase(iterUI);

			break;
		}
		++iter;
		++iterUI;
	}
	

	// 목록에 소환 될 유닛 제거
	CUnitMgr::Get()->DeletePSummonIndx(_Name);

	// UI위치 재정렬
	Vec2	PUIPos = Vec2(20.f, 10.f);
	for (int i = 0; i < m_UnitUI.size(); ++i)
	{
		m_UnitUI[i]->SetPos(PUIPos.x + (i * 125.f), PUIPos.y);

	}

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
}

void CLevel_Select::SetSummonUnitUI()
{
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();
	Vec2	PUIPos = Vec2(20.f, 10.f);
	for (int i = 0; i < UnitIndx.size(); ++i)
	{
		CBtnUI* newUI = new CBtnUI;

		newUI->SetScale(Vec2(107.f, 92.f));
		newUI->SetPos(PUIPos.x + (i * 125.f), PUIPos.y);
		newUI->SetNormalImg(UnitIndx[i]->GetIconTexture1(), UnitIndx[i]->GetIconLPos(), UnitIndx[i]->GetIconSize());
		newUI->SetPressedImg(UnitIndx[i]->GetIconTexture1(), UnitIndx[i]->GetIconLPos(), UnitIndx[i]->GetIconSize());
		newUI->SetOffImg(UnitIndx[i]->GetIconTexture2(), UnitIndx[i]->GetIconLPos(), UnitIndx[i]->GetIconSize());
		newUI->SetDelegate2(this, (BaseFunc_Param2)&CLevel_Select::ClickSummon, (DWORD_PTR)newUI, (DWORD_PTR)UnitIndx[i]->GetPUnitName());

		m_UnitUI.push_back(newUI);
		pPanel->AddChildUI(newUI);
	}
}

void CLevel_Select::SetSelectUnitUI()
{
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();
	Vec2	PUIPos = Vec2(10.f, 10.f);
	int Col = 0;
	int Row = 0;
	for (int i = 0; i < (int)PLAYER_UNIT_NAME::END; ++i)
	{
		if (3 <= Row)
		{
			Row = 0;
			++Col;
			if (Col == 4)
			{
				++m_Unitpage;
				Col = 0;
			}
		}

		CUnit* UnitIndx = CUnitMgr::Get()->GetPUnit((PLAYER_UNIT_NAME)i);
		CBtnUI* newUI = new CBtnUI;
		CBtnUI* newchildUI = new CBtnUI;

		newchildUI->SetScale(Vec2(80.f, 80.f));
		newchildUI->SetPos(PUIPos.x + (Row * 280.f), PUIPos.y + (Col * 120.f) + (m_Unitpage * 2000.f));
		newchildUI->SetNormalImg(UnitIndx->GetIconTexture1(), Vec2(0.f, 320.f), UnitIndx->GetIconSize());
		newchildUI->SetPressedImg(UnitIndx->GetIconTexture2(), Vec2(0.f, 320.f), UnitIndx->GetIconSize());
		newchildUI->SetOffImg(UnitIndx->GetIconTexture2(), Vec2(0.f, 320.f), UnitIndx->GetIconSize());
		m_SelectPanel->AddChildUI(newchildUI);
		m_SelectUnitUI.push_back(newchildUI);

		newUI->SetScale(Vec2(80.f, 80.f));
		newUI->SetPos(0.f,0.f);
		newUI->SetNormalImg(UnitIndx->GetIconTexture1(), UnitIndx->GetIconLPos(), UnitIndx->GetIconSize());
		newUI->SetPressedImg(UnitIndx->GetIconTexture2(), UnitIndx->GetIconLPos(), UnitIndx->GetIconSize());
		newUI->SetOffImg(UnitIndx->GetIconTexture2(), UnitIndx->GetIconLPos(), UnitIndx->GetIconSize());
		newUI->SetDelegate1(this, (BaseFunc_Param1)&CLevel_Select::ClickSelect, (DWORD_PTR)UnitIndx->GetPUnitName());
		newchildUI->AddChildUI(newUI);

		// 만일 현재 등록된 유닛이라면 비활성화 시켜주기
		UnitIndx = CUnitMgr::Get()->SearchPUnitByName(UnitIndx->GetPUnitName());
		if (UnitIndx)
		{
			newUI->SetBtnstat(false);
		}


		++Row;

	}
}

void CLevel_Select::NextUnitUI()
{

	// 적절한 움직임인지 확인
	++m_CurPage;
	if (m_CurPage > m_Unitpage)
	{
		m_CurPage = m_Unitpage;
		return;
	}

	// 다음유닛 페이지 이동(y축 -2000)

	for (int i = 0; i < m_SelectUnitUI.size(); ++i)
	{
		Vec2 vUiPos = m_SelectUnitUI[i]->GetPos();
		vUiPos.y -= 2000.f;
		m_SelectUnitUI[i]->SetPos(vUiPos);
	}

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
}

void CLevel_Select::PrevUnitUI()
{
	// 적절한 움직임인지 확인
	--m_CurPage;
	if (m_CurPage < 0)
	{
		m_CurPage = 0;
		return;
	}

	// 다음유닛 페이지 이동(y축 2000)

	for (int i = 0; i < m_SelectUnitUI.size(); ++i)
	{
		Vec2 vUiPos = m_SelectUnitUI[i]->GetPos();
		vUiPos.y += 2000.f;
		m_SelectUnitUI[i]->SetPos(vUiPos);
	}

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
}

void CLevel_Select::TextRender()
{
	SetTextColor(BackDC, RGB(255, 255, 255));
	// 코스트에 따른 숫자 텍스트배치
	vector<CUnit*>& UnitIndx = CUnitMgr::Get()->GetPSummonIndx();

	if (m_UnitUI.size())
	{
		Vec2 m_CostTextPos = m_UnitUI[0]->GetFinalPos();

		for (int i = 0; i < m_UnitUI.size(); ++i)
		{
			SetTextAlign(BackDC, TA_CENTER);
			SetBkMode(BackDC, TRANSPARENT);
			SELECT_FONT(BackDC, FONT_TYPE::DEFALT)
				wstring FUNDtext = std::to_wstring(UnitIndx[i]->GetStat().FUND);
			TextOutW(BackDC, (int)m_CostTextPos.x + (125.f * i) + 52, (int)m_CostTextPos.y + 102, FUNDtext.c_str(), FUNDtext.length());
		}
	}

	// 선택창 텍스쳐 배치
	for (int i = 0; i < m_SelectUnitUI.size(); ++i)
	{
		Vec2 m_SelectPos = m_SelectUnitUI[i]->GetFinalPos();
		CUnit* CurUnit = CUnitMgr::Get()->GetPUnit((PLAYER_UNIT_NAME)i);
		SetTextAlign(BackDC, DT_LEFT);
		SetBkMode(BackDC, TRANSPARENT);
		SELECT_FONT(BackDC, FONT_TYPE::LARGEFONT)

		wstring Namestr = CurUnit->GetName();
		TextOutW(BackDC, (int)m_SelectPos.x + 100, (int)m_SelectPos.y, Namestr.c_str(), Namestr.length());
		wstring FUNDtext = L"FUND  ";
		FUNDtext += std::to_wstring(CurUnit->GetStat().FUND);
		TextOutW(BackDC, (int)m_SelectPos.x + 100, (int)m_SelectPos.y + 50, FUNDtext.c_str(), FUNDtext.length());
	}
}















void CLevel_Select::NextLevel()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(CLevelMgr::Get()->GetStageSelectLayer());
}

void CLevel_Select::PrevLevel()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(LEVEL_TYPE::STAGE_SELECT);
}
