#include "pch.h"
#include "LeftBunker.h"

#include "CEngine.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CSearchRange.h"

LeftBunker::LeftBunker()
{
	m_MaxFund = 2000.f;
	m_HaveFund = 1000.f;

	SetScale(144.f, 491.f);
	SetTexture(CAssetMgr::Get()->LoadTexture(L"LeftBase", L"texture\\stage\\mapZone0.png"));

	m_Range->SetOffset(170.f, 28.f);
	m_Range->SetSearchLayer(LAYER_TYPE::ENEMY_UNIT);

	m_Defencetex = CAssetMgr::Get()->LoadTexture(L"LBaseDefence", L"texture\\stage\\mapUnit0_0.png");
}

LeftBunker::~LeftBunker()
{
}

void LeftBunker::Tick()
{
	// 사격 딜레이 계산
	m_BaseStat.AttackDuration += DT;

	// 코스트 회복
	m_FUNDRate += DT;
	if (m_FUNDDuration < m_FUNDRate)
	{
		FlusFUND(m_FUNDHeal);
		m_FUNDRate -= m_FUNDDuration;
	}
}

void LeftBunker::Render()
{
	CBunker::Render();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;


	Vec2 RenderPos = GetRenderPos();
	Vec2 ScalePos = GetScale();
	RenderPos += Vec2(42.f, -135.f);

	for (int i = 0; i < 3; ++i)
	{

		AlphaBlend(BackDC, (int)RenderPos.x - m_Defencetex->GetWidth()/2, (int)RenderPos.y + (93 * i) - m_Defencetex->GetHeight()/2
			, (int)m_Defencetex->GetWidth(), (int)m_Defencetex->GetHeight()
			, m_Defencetex->GetDC()
			, 0, 0
			, m_Defencetex->GetWidth(), m_Defencetex->GetHeight()
			, blend);
	}
}
