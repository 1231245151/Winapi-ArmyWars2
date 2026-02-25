#include "pch.h"
#include "CMap.h"

#include "CEngine.h"
#include "CTexture.h"

CMap::CMap()
	: m_MapInfo()
	, m_PosRate(1.f)
{
}

CMap::~CMap()
{
}

float CMap::GetLegth()
{
	return GetScale().x * m_MapInfo.PlatCol;
}

int CMap::SetIndx(int _Indx)
{
	if (_Indx < 0)
	{
		m_MapInfo.MapIndx = 4;
	}
	else if (_Indx > 4)
	{
		m_MapInfo.MapIndx = 0;
	}
	else
	{
		m_MapInfo.MapIndx = _Indx;
	}

	return m_MapInfo.MapIndx;
}

void CMap::Render()
{
	for (int i = 0; i < m_MapInfo.PlatCol; ++i)
	{

		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
		blend.AlphaFormat = AC_SRC_ALPHA;

		// 랜더링 할 위치 결정
		Vec2 RenderPos = Vec2(GetRenderPos().x + (GetScale().x * i), GetRenderPos().y);
		Vec2 ScalePos = GetScale();

		// 알파 블랜딩 랜더링
		AlphaBlend(BackDC, (int)(RenderPos.x * m_PosRate), (int)(RenderPos.y * m_PosRate)
			, (int)(ScalePos.x * m_PosRate), (int)(ScalePos.y * m_PosRate)
			, m_MapInfo.MapTexture->GetDC()
			, (int)ScalePos.x * (m_MapInfo.MapIndx), 0
			, (int)ScalePos.x , (int)ScalePos.y
			, blend);

	}

	// 구별용 ractagle 랜더링
	//Vec2 Pos = GetRenderPos();
	//Vec2 Scale = GetScale();
	//Vec2 Posrender = Vec2(Pos.x + (Scale.x / 2.f) * m_MapInfo.PlatCol, Pos.y + Scale.y / 2.f);
	//Vec2 Scalerender = Vec2((Scale.x * m_MapInfo.PlatCol), Scale.y);
	//
	//DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::RED, BRUSH_TYPE::HOLLOW, Posrender, Scalerender);
}
