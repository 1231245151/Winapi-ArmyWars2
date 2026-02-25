#pragma once
#include "CLevel.h"

class CUI;
class CBtnUI;
class CHoldUI;
class CDragUI;
class CTexture;

class CLevel_StgSelect :
    public CLevel
{
private:
    CBtnUI* m_PrevStage;
    CHoldUI* m_StageNameTexture;


    vector<CBtnUI*> m_StageBtnUI;


    CTexture* m_StageName;

public:
    void NextLevel(LEVEL_TYPE _Level);
    void PrevLevel();

public:
    virtual void Init() override;
    virtual void Exit() override;
    virtual void Tick() override;
    virtual void Render() override;

public:
    CLevel_StgSelect();
    ~CLevel_StgSelect();
};

