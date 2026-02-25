#pragma once
#include "CLevel.h"

enum class WIN_LOSE
{
	WIN,
	LOSE,
	NONE,
};

class CMap;
class CMiniMap;
class CTexture;
class LeftBunker;
class RightBunker;
class CPlatform;
class CUnit;
class CUI;
class CBtnUI;
class CHoldUI;
class CDragUI;
class CTextUI;
class CSkill;

class CLevel_Stage :
    public CLevel
{

protected:
	CMap*		m_BackETC;
	CMap*		m_BackMap;
	CMap*		m_PlatMap;
	CPlatform*	m_Platform;

	Vec2		m_MapPosBack;
	Vec2		m_MapPosBackE;
	Vec2		m_MapPlat;

	LeftBunker* pBunker;
	RightBunker* eBunker;

	Vec2			m_pBunkerPos;		// 왼쪽 벙커 위치
	Vec2			m_eBunkerPos;		// 오른쪽 벙커 위치

	UINT	m_MapCol;		// 맵 개수 선택
	int		m_MapIndx;		// 맵 디자인 선택

	float	m_LineLegth;		// 베이스와 베이스 기준 거리며 유닛의 이동될 기준
	Vec2	m_CostTextPos;
	Vec2	m_CameraLook;		// 시작 시 카메라 위치

	WIN_LOSE m_WinLose;

protected:
	// UI 주소 정보
	CHoldUI* pPanel;
	CBtnUI* m_FUNDUpgrade;
	CBtnUI* m_CooltimeUpgrade;
	CBtnUI* m_Changemenu;


	vector<CDragUI*> m_UnitUI;


	CMiniMap* m_MiniMap;

	// 기타 텍스쳐 정보
	CTexture* m_WinTex;
	CTexture* m_LoseTex;
	CTexture* m_VSTex;
	CTexture* m_CooltimeTex;

	// Upgrade 정보
	int		m_FUNDUpgradeCost;
	int		m_FUNDCount;
	int		m_CoolUpgradeCost;
	int		m_CoolCount;

	// 스킬 정보 UI
	vector<CSkill*> m_SkillIndx;
	vector<CUI*>	m_SkillUI;
	CBtnUI* m_RepairUI;
	CTextUI* m_RepaircostTex;

	CDragUI* m_AirStrikeUI;
	CTextUI* m_AirStrikecostTex;

	CDragUI* m_PrecisionBoomUI;
	CTextUI* m_PrecisionBoomTex;

	vector<CHoldUI*> m_vecKillUI;

	// 일시정지 UI
	CBtnUI* m_StopUI;
	CHoldUI* m_PauseUI;
	CBtnUI* m_RestartUI;
	CBtnUI* m_ChangeMenuUI;

public:
	virtual void LoadMap(const wstring& _RelativePath);

	// upgrade 함수
	virtual void UpgradeFUND();
	virtual void UpgradeCool();

	// 소환
	virtual void SommonUnit(PLAYER_UNIT_NAME _Unit);

	Vec2 GetStageLength();
	Vec2 GetLTopPos();

	// 일시정지, 해제
	void StopGame();
	void RestartGame();

protected:
	virtual void Init() override;
	virtual void Exit() override;
	virtual void Tick() override;
	virtual void FinalTick() override;
	virtual void Render() override;

	virtual void SetSummonUnitUI();
	void	SetSkillUI();
	virtual void UIOnOffjudge();
	void		Changestartmenu();
	void		KillUI();
	

public:
	CLevel_Stage();
	~CLevel_Stage();
};

