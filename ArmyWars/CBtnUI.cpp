#include "pch.h"
#include "CBtnUI.h"

#include "Resource.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CTexture.h"

CBtnUI::CBtnUI()
	: m_CallBack(nullptr)
	, m_Inst(nullptr)
	, m_MemFunc(nullptr)
	, m_MemFunc_1(nullptr)
	, m_MemFunc_2(nullptr)
	, m_NormalImg(nullptr)
	, m_PressedImg(nullptr)
	, m_OffImg(nullptr)
	, m_ParentBtnOn(true)
{
}

CBtnUI::~CBtnUI()
{
}

LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CBtnUI::LBtnDown()
{

}

void CBtnUI::LBtnClicked()
{
	if (m_BtnOn)
	{
		if (m_CallBack)
		{
			m_CallBack();
		}

		if (m_Inst && m_MemFunc)
		{
			(m_Inst->*m_MemFunc)();
		}

		if (m_Inst && m_MemFunc_1)
		{
			(m_Inst->*m_MemFunc_1)(m_DWORD1);
		}

		if (m_Inst && m_MemFunc_2)
		{
			(m_Inst->*m_MemFunc_2)(m_DWORD1,m_DWORD2);
		}
	}
}

void CBtnUI::SetNormalImg(CTexture* _Atlas)
{
	m_NormalImg = _Atlas;
	m_NorLTopPos = Vec2(0,0);
	m_NorScale = Vec2(m_NormalImg->GetWidth(), m_NormalImg->GetHeight());
}

void CBtnUI::SetPressedImg(CTexture* _Atlas)
{
	m_PressedImg = _Atlas;
	m_PresseLTopPos = Vec2(0, 0);
	m_PresseScale = Vec2(m_PressedImg->GetWidth(), m_PressedImg->GetHeight());
}

void CBtnUI::SetOffImg(CTexture* _Atlas)
{
	m_OffImg = _Atlas;
	m_OffLTopPos = Vec2(0, 0);
	m_OffScale = Vec2(m_OffImg->GetWidth(), m_OffImg->GetHeight());
}

void CBtnUI::SetNormalImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale)
{
	m_NormalImg = _Atlas;
	m_NorLTopPos = _LTopPos;
	m_NorScale = _Scale;
}

void CBtnUI::SetPressedImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale)
{
	m_PressedImg = _Atlas;
	m_PresseLTopPos = _LTopPos;
	m_PresseScale = _Scale;
}

void CBtnUI::SetOffImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale)
{
	m_OffImg = _Atlas;
	m_OffLTopPos = _LTopPos;
	m_OffScale = _Scale;
}

void CBtnUI::Tick_UI()
{
	// 자식 UI이며 활동 상태가 아니라면
	if (nullptr != GetParentUI() && !IsBtnOn())
	{
		// 자식 UI비활성화
		GetParentUI()->SetBtnstat(false);
	}
	// 자식 UI이며 활동 상태라면
	else if (nullptr != GetParentUI() && IsBtnOn())
	{
		// 자식 UI활성화
		GetParentUI()->SetBtnstat(true);
	}
}

void CBtnUI::Render_UI()
{


	CTexture* RenderImg = nullptr;
	Vec2 RenderPos = Vec2(0.f,0.f);
	Vec2 RenderScale = Vec2(0.f, 0.f);
	if (m_BtnOn && !IsLBtnDown())
	{
		RenderImg = m_NormalImg;
		RenderPos = m_NorLTopPos;
		RenderScale = m_NorScale;
	}
	else if (m_BtnOn)
	{
		RenderImg = m_PressedImg;
		RenderPos = m_PresseLTopPos;
		RenderScale = m_PresseScale;
	}
	else
	{
		RenderImg = m_OffImg;
		RenderPos = m_OffLTopPos;
		RenderScale = m_OffScale;
	}

	if (nullptr == RenderImg)
	{
		if (nullptr == m_NormalImg)
		{
			return;
		}
		else
		{
			RenderImg = m_NormalImg;
		}
	}
	
	Vec2 vFinalPos = GetFinalPos();
	Vec2 vScale = GetScale();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
		, GetScale().x, GetScale().y
		, RenderImg->GetDC()
		, RenderPos.x, RenderPos.y
		, RenderScale.x, RenderScale.y
		, blend);


	//Rectangle(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
	//	, (int)vFinalPos.x + (int)vScale.x, (int)vFinalPos.y + (int)vScale.y);
	

	//TransparentBlt(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
	//	, (int)vFinalPos.x + (int)vScale.x, (int)vFinalPos.y + (int)vScale.y
	//	, m_NormalImg->GetDC()
	//	, 0, 0
	//	, m_NormalImg->GetWidth(), m_NormalImg->GetHeight()
	//	, RGB(255, 0, 255));
}