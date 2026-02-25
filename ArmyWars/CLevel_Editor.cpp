#include "pch.h"
#include "CLevel_Editor.h"

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
#include "CAirSupport.h"

#include "CUIMgr.h"
#include "CBtnUI.h"
#include "CHoldUI.h"

#include "resource.h"


LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void TestFunc2()
{
	ChangeLevel(LEVEL_TYPE::START);
}

HWND hDig = nullptr;

CLevel_Editor::CLevel_Editor()
	: m_hMenu(nullptr)
	, pBunker(nullptr)
	, eBunker(nullptr)
	, m_LineLegth(1000.f)
	, m_MapCol(2)
	, m_MapIndx(0)
	, m_CameraLook(640.f,820.f)
{
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_ARMYWARS));
}

CLevel_Editor::~CLevel_Editor()
{
	DestroyMenu(m_hMenu);
}


void CLevel_Editor::Init()
{
	// 카메라 이동
	CCamera::Get()->SetLookAt(m_CameraLook);
	// 카메라 명령
	CCamera::Get()->SetCameraStatus(COMMANDALLOPEN);

	// 카메라 고정 해제
	CCamera::Get()->SetCameraUPDOWNLock(false);
	CCamera::Get()->SetCameraLRLock(false);

	// 메뉴 추가	
	SetMenu(CEngine::Get()->GetMainHwnd(), m_hMenu);

	// 메뉴가 붙었으니까, 다시 윈도우 크기 재계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);



	// Map Object 배치
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
	m_eBunkerPos = Vec2((m_LineLegth + pBunker->GetScale().x + eBunker->GetScale().x / 2.f), (m_PlatMap->GetScale().y / 2.f) + m_PlatMap->GetPos().y - 28.f);
	eBunker->SetPos(m_eBunkerPos);

	AddObject(eBunker, LAYER_TYPE::ENEMY_BASE);
}

void CLevel_Editor::Exit()
{
	// 카메라 고정
	CCamera::Get()->SetCameraUPDOWNLock(true);
	CCamera::Get()->SetCameraLRLock(true);

	// 메뉴 제거
	SetMenu(CEngine::Get()->GetMainHwnd(), nullptr);

	// 메뉴가 제거되었으니, 윈도우 크기 다시 계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);


	// 오브젝트 삭제
	DeleteAllObjects();
}

void CLevel_Editor::Tick()
{
	CLevel::Tick();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::START);
	}
}






















// ===============================


void CLevel_Editor::TileSet(float _Legth)
{
	m_LineLegth = _Legth;
	eBunker->SetPos(pBunker->GetScale().x + m_LineLegth + eBunker->GetScale().x / 2.f, eBunker->GetPos().y);
	m_eBunkerPos = eBunker->GetPos();

	float MapLegth = m_PlatMap->GetLegth();


	if (m_LineLegth > MapLegth)
	{
		m_MapCol = floor((m_LineLegth + eBunker->GetScale().x + pBunker->GetScale().x) / m_PlatMap->GetScale().x) + 1;
		m_BackMap->SetPlatCol(m_MapCol);
		m_BackETC->SetPlatCol(m_MapCol);
		m_PlatMap->SetPlatCol(m_MapCol);

	}
	else if (m_LineLegth < MapLegth)
	{
		m_MapCol = floor((m_LineLegth + eBunker->GetScale().x + pBunker->GetScale().x) / m_PlatMap->GetScale().x) + 1;
		m_BackMap->SetPlatCol(m_MapCol);
		m_BackETC->SetPlatCol(m_MapCol);
		m_PlatMap->SetPlatCol(m_MapCol);
	}

}

void CLevel_Editor::MapChange(int _info)
{
	m_MapIndx = m_BackMap->SetIndx(_info);
	m_MapIndx = m_BackETC->SetIndx(_info);
	m_MapIndx = m_PlatMap->SetIndx(_info);
}


void CLevel_Editor::SaveMap()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"MAP\0*.map\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, szFilePath, L"w");

			// 맵의 정보
			fwprintf_s(pFile, L"[MAP_SELECT]\n");
			fwprintf_s(pFile, L"Map_Indx	%d\n", m_MapIndx);
			fwprintf_s(pFile, L"Map_Col		%d\n", m_MapCol);
			fwprintf_s(pFile, L"Map_Legth	%f\n", m_LineLegth);
			fwprintf_s(pFile, L"Map_Pos		%f %f %f %f %f %f\n", m_BackMap->GetPos().x, m_BackMap->GetPos().y, m_BackETC->GetPos().x, m_BackETC->GetPos().y, m_PlatMap->GetPos().x, m_PlatMap->GetPos().y );
			fwprintf_s(pFile, L"END	\n");
			fwprintf_s(pFile, L"\n");

			// 기지의 위치 정보
			fwprintf_s(pFile, L"[BASE_POS]\n");
			fwprintf_s(pFile, L"BASE_LEFT	%f  %f\n", m_pBunkerPos.x, m_pBunkerPos.y);
			fwprintf_s(pFile, L"BASE_RIGHT	 %f  %f\n", m_eBunkerPos.x, m_eBunkerPos.y);
			fwprintf_s(pFile, L"\n");



			fclose(pFile);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Editor::LoadMap()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"MAP\0*.map\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{

			FILE* pFile = nullptr;
			_wfopen_s(&pFile, szFilePath, L"r");

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
	}



	CUIMgr::Get()->SetPreventFrame(1);
}














// ===============================
// 타일관련 다이얼로그 프로시저 함수
// ===============================
LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_CLOSE:
		DestroyWindow(hDig);
		break; 

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			float Legth = GetDlgItemInt(hWnd, IDC_LEGTH, nullptr, true);

			if (Legth <= 1000.f)
			{
				MessageBox(nullptr, L"길이는 1000 이하로 설정 할 수 없습니다..", L"타일 행, 렬 설정 오류", MB_OK);
				return (INT_PTR)TRUE;
			}

			// 1. 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);

			// 2. 레벨 에디터 타일 설정 함수로 접근
			pEditorLevel->TileSet(Legth);


			//DestroyWindow(hDig);
			//EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			DestroyWindow(hDig);
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_SAVE)
		{
			// 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
			
			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);

			pEditorLevel->SaveMap();


			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_LOAD)
		{
			// 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);

			pEditorLevel->LoadMap();


			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_MAPNEXT)
		{
			// 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);

			UINT indx = pEditorLevel->GetMapIndx();
			pEditorLevel->MapChange(++indx);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_MAPPREV)
		{
			// 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);

			UINT indx = pEditorLevel->GetMapIndx();
			pEditorLevel->MapChange(--indx);


			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON1)
		{
			// 현재 레벨에 접근
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pEditorLevel);


			CCamera::Get()->SetLookAt(pEditorLevel->GetCameraLook());

			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}
