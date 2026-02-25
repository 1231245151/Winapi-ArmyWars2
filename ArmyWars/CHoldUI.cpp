#include "pch.h"
#include "CHoldUI.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CLogMgr.h"
#include "CAssetMgr.h"
#include "CTexture.h"

CHoldUI::CHoldUI()
	: m_NormalImg(nullptr)
{
}

CHoldUI::~CHoldUI()
{
}

void CHoldUI::Tick_UI()
{
}

void CHoldUI::Render_UI()
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
		, RenderPos.x, RenderPos.y
		, RenderScale.x, RenderScale.y
		, blend);
}

void CHoldUI::LBtnDown()
{
}

void CHoldUI::SetNormalImg(CTexture* _Atlas, Vec2 _RenderPos, Vec2 _RenderScale)
{
	m_NormalImg = _Atlas;
	RenderPos = _RenderPos;
	if (_RenderScale == Vec2(0.f, 0.f))
	{
		RenderScale.x = m_NormalImg->GetWidth();
		RenderScale.y = m_NormalImg->GetHeight();
	}
	else
	{
		RenderScale = _RenderScale;
	}
}