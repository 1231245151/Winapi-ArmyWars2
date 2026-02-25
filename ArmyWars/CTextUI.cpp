#include "pch.h"
#include "CTextUI.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CLogMgr.h"
#include "CAssetMgr.h"
#include "CTexture.h"


CTextUI::CTextUI()
	:m_TextOffset(0.f,0.f)
{
}

CTextUI::~CTextUI()
{
}

void CTextUI::Tick_UI()
{
}

void CTextUI::Render_UI()
{
	Vec2 vFinalPos = GetFinalPos();
	Vec2 vScale = GetScale();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
		, GetScale().x, GetScale().y
		, m_NormalImg->GetDC()
		, 0, 0
		, m_NormalImg->GetWidth(), m_NormalImg->GetHeight()
		, blend);

	SELECT_FONT(BackDC, FONT_TYPE::DEFALT)
	SetTextColor(BackDC, RGB(255, 255, 255));
	SetTextAlign(BackDC, TA_CENTER);
	SetBkMode(BackDC, TRANSPARENT);
	TextOut(BackDC, (int)(vFinalPos.x + GetScale().x/2.f + m_TextOffset.x), (int)(vFinalPos.y + GetScale().y / 2.f + m_TextOffset.y), m_Text.c_str(), m_Text.length());

}