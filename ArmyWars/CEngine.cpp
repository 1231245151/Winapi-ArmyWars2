#include "pch.h"
#include "CEngine.h"

#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CLevel.h"
#include "CPathMgr.h"
#include "CDebugMgr.h"
#include "CTaskMgr.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CAssetMgr.h"
#include "CLogMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CUnitMgr.h"

#include "CTexture.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_DC(nullptr)
	, m_Pen{}
	, m_Brush{}
	, m_RealResol()
	, m_Resolution()
{
}

CEngine::~CEngine()
{
	ReleaseDC(m_hMainWnd, m_DC);



	for (int i = 0; i < (int)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_Pen[i]);
	}

	for (int i = 0; i < (int)BRUSH_TYPE::HOLLOW; ++i)
	{
		DeleteObject(m_Brush[i]);
	}

	for (int i = 0; i < (int)FONT_TYPE::END; ++i)
	{
		DeleteObject(m_Font[i]);
	}
}

void CEngine::Init(HWND _MainHwnd, UINT _Width, UINT _Height)
{
	m_hMainWnd = _MainHwnd;

	// DC(Device Context)
	// 화면에 그리기(Rendering) 작업을 담당하는 장치
	// 렌더링에 필요한 데이터 집합
	// Pen(Black), Brush(White), Bitmap
	m_DC = GetDC(m_hMainWnd);

	// 윈도우 해상도 적용
	ChangeWindowResolution(_Width, _Height);

	// 백버퍼 용 텍스쳐 생성
	m_BackBuffer = CAssetMgr::Get()->CreateTexture(L"BackBufferTex", _Width, _Height);

	// 자주 사용할 펜 및 브러쉬 생성
	CreateGDIObject();

	// Manager 초기화
	CPathMgr::Get()->Init();
	CTimeMgr::Get()->Init();
	CKeyMgr::Get()->Init();
	CSoundMgr::Get()->Init();
	CUnitMgr::Get()->Init();
	CLevelMgr::Get()->Init();
	CCamera::Get()->Init();

	m_BackGrandBuffer = CAssetMgr::Get()->LoadTexture(L"BackTexture", L"texture\\start\\menuBg.png");
}


void CEngine::Progress()
{
	CTimeMgr::Get()->Tick();
	CCamera::Get()->Tick();
	CKeyMgr::Get()->Tick();
	CUnitMgr::Get()->Tick();
	CLevelMgr::Get()->Tick();
	CCollisionMgr::Get()->Tick();
	CUIMgr::Get()->Tick();

	// Level 물체들 렌더링
	Render();

	// 다음 프레임에 적용되야 할 작업들을 처리
	CTaskMgr::Get()->Tick();
}

void CEngine::Render()
{
	// Rendering
	// 윈도우 화면(비트맵) 을 특정 색상으로 Clear 해준다.
	SELECT_BRUSH(m_BackBuffer->GetDC(), BRUSH_TYPE::GRAY);
	Rectangle(m_BackBuffer->GetDC(), -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);
	TransparentBlt(m_BackBuffer->GetDC(),
		0, 0,
		m_Resolution.x, m_Resolution.y,
		m_BackGrandBuffer->GetDC(),
		0, 0,
		m_BackGrandBuffer->GetWidth(), m_BackGrandBuffer->GetHeight(),
		RGB(255, 0, 255));

	// 레벨에 물체들을 화면에 렌더링
	CLevelMgr::Get()->Render();

	// Debug 렌더링
	CDebugMgr::Get()->Tick();

	// Camera 렌더링
	CCamera::Get()->Render();

#ifdef _DEBUG
	// Log 렌더링
	CLogMgr::Get()->Tick();
#endif

	// 보고싶은 값 호출용
	//wchar_t CurDir[256] = {};
	//GetCurrentDirectory(256, CurDir);
	//swprintf_s(CurDir, 256, L"왼쪽유닛 발사체: %f, 오른쪽 유닛 발사체 :%f", test1, test2);
	//bool ttt = TextOut(GetBackDC(), 100, 100, CurDir, wcslen(CurDir));

	// BackBuffer 그림을 MainDC 버퍼로 복사
	BitBlt(m_DC, 0, 0, (int)m_Resolution.x, (int)m_Resolution.y, m_BackBuffer->GetDC(), 0, 0, SRCCOPY);
}

void CEngine::CreateGDIObject()
{
	m_Pen[(int)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_Pen[(int)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_Pen[(int)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_Pen[(int)PEN_TYPE::YELLOW] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	m_Pen[(int)PEN_TYPE::MAGENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));


	m_Brush[(int)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_Brush[(int)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	m_Brush[(int)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_Brush[(int)BRUSH_TYPE::GRAY] = CreateSolidBrush(RGB(100, 100, 100));
	m_Brush[(int)BRUSH_TYPE::TURQUOISE] = CreateSolidBrush(RGB(57, 215, 237));
	m_Brush[(int)BRUSH_TYPE::WHITE] = CreateSolidBrush(RGB(255, 255, 255));
	m_Brush[(int)BRUSH_TYPE::BLACK] = CreateSolidBrush(RGB(0, 0, 0));
	m_Brush[(int)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	m_Font[(int)FONT_TYPE::DEFALT] = CreateFontW(0, 0, 0, 0, FW_BOLD, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Defalt_Font");

	m_Font[(int)FONT_TYPE::MEDIUMFONT] = CreateFontW(25, 0, 0, 0, FW_BOLD, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Heavy_Font");

	m_Font[(int)FONT_TYPE::HEAVYFONT] = CreateFontW(30, 0, 0, 0, FW_BOLD, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Heavy_Font");

	m_Font[(int)FONT_TYPE::LARGEFONT] = CreateFontW(34, 0, 0, 0, FW_HEAVY, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_DONTCARE, L"Large_Font");
}

HDC CEngine::GetBackDC()
{
	return m_BackBuffer->GetDC();
}

void CEngine::ChangeWindowResolution(UINT _Width, UINT _Height)
{
	// 변경하고자 하는 해상도 정보를 Engine 객체에 저장
	m_Resolution.x = (float)_Width;
	m_Resolution.y = (float)_Height;

	// 크기를 변경하려는 윈도우의 메뉴 존재여부를 확인
	HMENU hMenu = GetMenu(m_hMainWnd);

	// 윈도우의 그리기영역(비트맵) 의 해상도를 원하는 수치로 만들기 위해서
	// 실제 윈도우가 가져야 하는 크기를 계산
	RECT rt = { 0, 0, _Width, _Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, (bool)hMenu);

	// 최종으로 계산된 윈도우 크기값으로 윈도우 크기를 변경
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	//SetWindowPos(m_hMainWnd, nullptr, 0, 0, _Width, _Height, 0);

}
