#include "pch.h"
#include "CBackMap.h"

#include "CAssetMgr.h"
#include "CEngine.h"
#include "CTexture.h"

CBackMap::CBackMap()
{
	m_MapInfo.MapTexture = CAssetMgr::Get()->LoadTexture(L"BackMapTex", L"texture\\stage\\mapField0_2.png");

	SetScale(1536.f, 1230.f);
	SetPos(0.f, 0.f);
	SetPosRate(1.f / 2.f);
}

CBackMap::~CBackMap()
{
}

void CBackMap::Render()
{
	for (int i = 0; i < m_MapInfo.PlatCol; ++i)
	{

		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
		blend.AlphaFormat = AC_SRC_ALPHA;

		// 랜더링 할 위치 결정
		Vec2 RenderPos = Vec2(GetRenderPos().x + (GetScale().x * i) - (200 * i), GetRenderPos().y);
		Vec2 ScalePos = GetScale();

		// 알파 블랜딩 랜더링
		AlphaBlend(BackDC, (int)(RenderPos.x * m_PosRate), (int)(RenderPos.y * m_PosRate) - 15.f
			, (int)(ScalePos.x * m_PosRate) -100, (int)(ScalePos.y * m_PosRate) -100
			, m_MapInfo.MapTexture->GetDC()
			, (int)ScalePos.x * (m_MapInfo.MapIndx), 0
			, (int)ScalePos.x, (int)ScalePos.y
			, blend);

	}
}
