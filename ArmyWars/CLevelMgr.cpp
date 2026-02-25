#include "pch.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"

#include "CTimeMgr.h"
#include "CLevel_Editor.h"
#include "CLevel_Stage.h"
#include "CLevel_Test.h"
#include "CLevel_Stage01.h"
#include "CLevel_Stage02.h"
#include "CLevel_Stage03.h"
#include "CLevel_Stage04.h"
#include "CLevel_Start.h"
#include "CLevel_Select.h"
#include "CLevel_StgSelect.h"
#include "CCamera.h"


CLevelMgr::CLevelMgr()
	: m_arrLevel{}
	, m_CurLevel(nullptr)
	, m_StageSelectLayer(LEVEL_TYPE::STAGE_TEST)
	, m_PrevLevel(nullptr)
	, m_PrevLayer(LEVEL_TYPE::START)
{}

CLevelMgr::~CLevelMgr()
{
	for (int i = 0; i < (int)LEVEL_TYPE::END; ++i)
	{
		if (nullptr != m_arrLevel[i])
			delete m_arrLevel[i];
	}
}

void CLevelMgr::Init()
{
	// MiniMap용 텍스쳐 생성
	CAssetMgr::Get()->CreateTexture(L"MiniMap",100, 100);

	// Level 생성
	m_arrLevel[(int)LEVEL_TYPE::START] = new CLevel_Start;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_TEST] = new CLevel_Test;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_01] = new CLevel_Stage01;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_02] = new CLevel_Stage02;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_03] = new CLevel_Stage03;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_04] = new CLevel_Stage04;
	m_arrLevel[(int)LEVEL_TYPE::EIDTOR] = new CLevel_Editor;
	m_arrLevel[(int)LEVEL_TYPE::UNIT_SELECT] = new CLevel_Select;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_SELECT] = new CLevel_StgSelect;

	m_CurLevel = m_arrLevel[(int)LEVEL_TYPE::START];
	m_CurLevel->Init();
}

void CLevelMgr::Tick()
{
	if (!m_Change)
	{
		m_CurLevel->Tick();
		m_CurLevel->FinalTick();
	}
	else if(CCamera::Get()->GetCameraStat() & DOORALLCLOSE)
	{
		CameraChange();
		m_Change = false;
	}
}

void CLevelMgr::Render()
{
	m_CurLevel->Render();
}

void CLevelMgr::PlayGameBGM(bool _continue)
{
	if(nullptr != m_GameBGM)
	{
		m_GameBGM->PlayToBGM(true, _continue);
	}
}

void CLevelMgr::StopGameBGM(bool _bReset)
{
	if (nullptr != m_GameBGM)
	{
		m_GameBGM->Stop(_bReset);
	}
}

void CLevelMgr::SetGameBGM(CSound* _BGM)
{
	m_GameBGM = _BGM;
}

void CLevelMgr::ChangeLevel(LEVEL_TYPE _Type)
{
	// 카메라 명령
	CCamera::Get()->SetDoorOn(true);
	CCamera::Get()->SetCameraStatus(COMMANDALLCLOSE);

	//CTimeMgr::Get()->SetTimeStop(false);

	m_Change = true;

	m_PrevLayer = m_LevelLayer;

	m_LevelLayer = _Type;
	m_PrevLevel = m_PrevLevel;
}

void CLevelMgr::CameraChange()
{
	// 변경 전의 레벨을 Exit
	if (nullptr != m_CurLevel)
	{
		m_CurLevel->Exit();
	}

	// 새로운 레벨을 가리키고, 초기화(Init) 을 해둔다.
	m_CurLevel = m_arrLevel[(int)m_LevelLayer];
	m_CurLevel->Init();

	// 틱이 멈춰있을 수 있으니 다시 틱 진행
	CTimeMgr::Get()->SetTimeStop(false);
}
