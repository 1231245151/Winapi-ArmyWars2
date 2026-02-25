#include "pch.h"
#include "CMiniMap.h"

#include "CEngine.h"
#include "CCamera.h"
#include "CAssetMgr.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLevel_Stage.h"
#include "CTexture.h"

#include "CBunker.h"
#include "CUnit.h"


CMiniMap::CMiniMap()
{
	SetScale(300, 100);
	m_BlackMap = CAssetMgr::Get()->FindTexture(L"MiniMap");
	m_EnemyRed = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");
	m_PlayerBlue = CAssetMgr::Get()->LoadTexture(L"RangeBlue", L"texture\\effect\\Range_Blue.png");
	m_UnitIconSize = Vec2(5.f, 5.f);

	SELECT_BRUSH(m_BlackMap->GetDC(), BRUSH_TYPE::BLACK)
	SELECT_PEN(m_BlackMap->GetDC(), PEN_TYPE::MAGENTA)
	Rectangle(m_BlackMap->GetDC(), -1, -1, (int)GetScale().x + 1, (int)GetScale().y + 1);
}

CMiniMap::~CMiniMap()
{
}

void CMiniMap::Tick()
{
}

void CMiniMap::Render()
{
	// 최초로는 알파 랜더링 되는 검은 화면을 띄움
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 150; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = 0;

	AlphaBlend(BackDC, (int)vPos.x, (int)vPos.y
		, vScale.x, vScale.y
		, m_BlackMap->GetDC()
		, 0, 0
		, m_BlackMap->GetWidth(), m_BlackMap->GetHeight()
		, blend);


	// 실제 맵의 x,y길이구하기
	// 실제 맵길이 / 해당 유닛의 위치 거리 계산, 이걸로 미니맵의 위치 비율
	
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CLevel_Stage* CurStage = dynamic_cast<CLevel_Stage*>(CurLevel);
	if (nullptr == CurStage)
	{
		LOG(LOG_LEVEL::BUG, L"올바른 레벨이 아니어서 미니맵 삭제")
		DeleteObject(this);
		return;
	}
	Vec2 RealLength = CurStage->GetStageLength();		//맵의 x,y길이
	Vec2 LevelPlat = CurStage->GetLTopPos();			//맵의 맨 좌측위 위치
	
	vector<CObj*>& EUnit = CurLevel->GetLayer(LAYER_TYPE::ENEMY_UNIT);
	vector<CObj*>& PUnit = CurLevel->GetLayer(LAYER_TYPE::PLAYER_UNIT);
	// 적 유닛 랜더링
	for (int i = 0; i < EUnit.size(); ++i)
	{
		Vec2 UnitPos = EUnit[i]->GetPos();
		float UnitLengthX = abs(LevelPlat.x - UnitPos.x);	// 유닛의 맵에서의 상대적 거리
		float UnitLengthY = abs(LevelPlat.y - UnitPos.y);

		if (UnitLengthX == 0.f)
		{
			UnitLengthX = 0.1f;
		}
		Vec2 RatioMap = Vec2(UnitLengthX / RealLength.x, UnitLengthY / RealLength.y);		// 맵상에서 위치상 비율


		AlphaBlend(BackDC, (int)vPos.x + (vScale.x * RatioMap.x), (int)vPos.y + (vScale.y * RatioMap.y)
			, m_UnitIconSize.x, m_UnitIconSize.y
			, m_EnemyRed->GetDC()
			, 0, 0
			, m_EnemyRed->GetWidth(), m_EnemyRed->GetHeight()
			, blend);

	}

	// 아군 유닛 랜더링
	for (int i = 0; i < PUnit.size(); ++i)
	{
		Vec2 UnitPos = PUnit[i]->GetPos();
		float UnitLengthX = abs(LevelPlat.x - UnitPos.x);	// 유닛의 맵에서의 상대적 거리
		float UnitLengthY = abs(LevelPlat.y - UnitPos.y);

		if (UnitLengthX == 0.f)
		{
			UnitLengthX = 0.1f;
		}
		Vec2 RatioMap = Vec2(UnitLengthX / RealLength.x, UnitLengthY / RealLength.y);	// 맵상에서 위치상 비율


		AlphaBlend(BackDC, (int)vPos.x + (vScale.x * RatioMap.x), (int)vPos.y + (vScale.y * RatioMap.y)
			, m_UnitIconSize.x, m_UnitIconSize.y
			, m_PlayerBlue->GetDC()
			, 0, 0
			, m_PlayerBlue->GetWidth(), m_PlayerBlue->GetHeight()
			, blend);

	}



	// 자신의 시야 위치 랜더링
	Vec2 CameraPos = CCamera::Get()->GetLookAt();
	Vec2 Resolustion = CEngine::Get()->GetResolution();
	float CameraLXPos = CameraPos.x - Resolustion.x / 2.f;
	if (CameraLXPos == 0.f)
	{
		CameraLXPos = 0.1f;
	}

	float ResolXScale = Resolustion.x  / RealLength.x;		// 실제 화면과 전장 길이의 비율
	float MapXScale = vScale.x * (ResolXScale);			// 미니맵에서 그려질 카메라 스케일

	float RatioMap = vScale.x / RealLength.x;


	SELECT_BRUSH(BackDC, BRUSH_TYPE::HOLLOW)
	SELECT_PEN(BackDC, PEN_TYPE::YELLOW)
	Rectangle(BackDC, (int)vPos.x + (CameraLXPos * RatioMap), vPos.y, (int)vPos.x + (CameraLXPos * RatioMap) + (MapXScale), (int)vPos.y + vScale.y);

}