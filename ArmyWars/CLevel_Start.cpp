#include "pch.h"
#include "CLevel_Start.h"

#include "CEngine.h"
#include "CLogMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CTexture.h"
#include "CAssetMgr.h"
#include "CPathMgr.h"
#include "CLevelMgr.h"

#include "CUIMgr.h"
#include "CBtnUI.h"
#include "CHoldUI.h"
#include "CDragUI.h"

CLevel_Start::CLevel_Start()
{
}

CLevel_Start::~CLevel_Start()
{
}


void CLevel_Start::Init()
{
	// 사운드
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"StartBGM", L"sound\\start_bg.wav");
	if (pBGM != CLevelMgr::Get()->GetGameBGM())
	{
		CLevelMgr::Get()->SetGameBGM(pBGM);
		CLevelMgr::Get()->PlayGameBGM(false);
	}

	// 카메라 명령
	CCamera::Get()->SetCameraStatus(COMMANDHALFOPEN);
	

	// 메뉴 제거
	SetMenu(CEngine::Get()->GetMainHwnd(), nullptr);

	// 메뉴가 제거되었으니, 윈도우 크기 다시 계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);

	// 버튼 등록
	m_StartBtn = new CBtnUI;
	m_StartBtn->SetName(L"Start Panel");
	m_StartBtn->SetScale(Vec2(510.f, 54.f));
	m_StartBtn->SetPos(Vec2(650.f, 210.f));
	m_StartBtn->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"StartPanel1", L"texture\\start\\menuKey1_00.png"));
	m_StartBtn->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"StartPanel2", L"texture\\start\\menuKey1_01.png"));
	m_StartBtn->SetOffImg(CAssetMgr::Get()->LoadTexture(L"StartPanel1", L"texture\\start\\menuKey1_00.png"));
	m_StartBtn->SetDelegate(this, (BaseFunc)&CLevel_Start::ChangeStage);
	AddObject(m_StartBtn, LAYER_TYPE::UI);

	m_EditorBtn = new CBtnUI;
	m_EditorBtn->SetName(L"Butten2 Panel");
	m_EditorBtn->SetScale(Vec2(510.f, 54.f));
	m_EditorBtn->SetPos(Vec2(650.f, 300.f));
	m_EditorBtn->SetNormalImg(CAssetMgr::Get()->LoadTexture(L"EditorPanel1", L"texture\\start\\menuKey1_30.png"));
	m_EditorBtn->SetPressedImg(CAssetMgr::Get()->LoadTexture(L"EditorPanel2", L"texture\\start\\menuKey1_31.png"));
	m_EditorBtn->SetOffImg(CAssetMgr::Get()->LoadTexture(L"EditorPanel1", L"texture\\start\\menuKey1_30.png"));
	m_EditorBtn->SetDelegate(this, (BaseFunc)&CLevel_Start::ChangeEditor);
	AddObject(m_EditorBtn, LAYER_TYPE::UI);
}

void CLevel_Start::Exit()
{
	DeleteAllObjects();
}

void CLevel_Start::Tick()
{

	CLevel::Tick();

}

void CLevel_Start::Render()
{
	CLevel::Render();
}


void CLevel_Start::ChangeEditor()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(LEVEL_TYPE::EIDTOR);
}

void CLevel_Start::ChangeStage()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"unknownvoice", L"sound\\unknown.wav");
	pBGM->Play();
	ChangeLevel(LEVEL_TYPE::STAGE_SELECT);
}