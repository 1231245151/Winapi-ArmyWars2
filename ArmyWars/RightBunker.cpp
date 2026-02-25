#include "pch.h"
#include "RightBunker.h"

#include "CEngine.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CTimeMgr.h"

#include "CCollider.h"
#include "CSearchRange.h"
#include "CStateMachine.h"
#include "EnemySommonState.h"

RightBunker::RightBunker()
{
	SetScale(144.f, 491.f);
	SetTexture(CAssetMgr::Get()->LoadTexture(L"RightBase", L"texture\\stage\\mapZone1.png"));

	m_Range->SetOffset(-170.f, 28.f);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);

	m_Defencetex = CAssetMgr::Get()->LoadTexture(L"RBaseDefence", L"texture\\stage\\mapUnit1_0.png");
}

RightBunker::~RightBunker()
{
}

void RightBunker::Tick()
{
	// 사격 딜레이 계산
	m_BaseStat.AttackDuration += DT;
}

void RightBunker::Render()
{
	CBunker::Render();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;


	Vec2 RenderPos = GetRenderPos();
	Vec2 ScalePos = GetScale();
	RenderPos += Vec2(-42.f, -135.f);

	for (int i = 0; i < 3; ++i)
	{


		AlphaBlend(BackDC, (int)RenderPos.x - m_Defencetex->GetWidth() / 2, (int)RenderPos.y + (93 * i) - m_Defencetex->GetHeight() / 2
			, (int)m_Defencetex->GetWidth(), (int)m_Defencetex->GetHeight()
			, m_Defencetex->GetDC()
			, 0, 0
			, m_Defencetex->GetWidth(), m_Defencetex->GetHeight()
			, blend);
	}
}

void RightBunker::HPUI()
{
	Vec2 RenderPos = Vec2(CEngine::Get()->GetResolution().x, 0.f);
	float HPrate = 0;
	if (0 < m_BaseStat.HP)
	{
		HPrate = (float)m_BaseStat.HP / (float)m_BaseStat.MaxHP;
	}
	else
	{
		HPrate = 0.f;
	}


	SELECT_BRUSH(BackDC, BRUSH_TYPE::BLUE);
	Vec2 vScale = Vec2(600.f, 20.f) * Vec2(HPrate, 1.f);

	Rectangle(BackDC, (int)RenderPos.x - (int)vScale.x, (int)RenderPos.y - (int)vScale.y
		, (int)RenderPos.x, (int)RenderPos.y + (int)vScale.y);
}
