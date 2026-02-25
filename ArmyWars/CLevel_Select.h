#pragma once
#include "CLevel.h"

class CUI;
class CBtnUI;
class CHoldUI;
class CDragUI;
class CTexture;

class CLevel_Select :
    public CLevel
{
private:
    CHoldUI*    pPanel;
    CHoldUI*    m_SelectPanel;
    CBtnUI*     m_NextStage;
    CBtnUI*     m_PrevStage;

    CBtnUI*     m_NextUnit;
    CBtnUI*     m_PrevUnit;

    vector<CBtnUI*> m_UnitUI;
    vector<CBtnUI*> m_SelectUnitUI;
    int     m_Unitpage;
    int     m_CurPage;

    CHoldUI* m_SelectPanelPlus;

public:
    void ClickSelectUI(PLAYER_UNIT_NAME _Name);     // 현재 더미데이터
    void ClickSelect(PLAYER_UNIT_NAME _Name);
    void ClickSummon(CUI* _UI, PLAYER_UNIT_NAME _Name);

private:
    void SetSummonUnitUI();
    void SetSelectUnitUI();
    void NextUnitUI();
    void PrevUnitUI();

    void TextRender();

    void NextLevel();
    void PrevLevel();

public:
    virtual void Init() override;
    virtual void Exit() override;
    virtual void Tick() override;
    virtual void Render() override;

public:
    CLevel_Select();
    ~CLevel_Select();
};

