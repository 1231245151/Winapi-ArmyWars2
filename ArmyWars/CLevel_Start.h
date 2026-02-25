#pragma once
#include "CLevel.h"


class CBtnUI;
class CHoldUI;
class CDragUI;
class CTexture;


class CLevel_Start :
    public CLevel
{
    CBtnUI* m_StartBtn;
    CBtnUI* m_EditorBtn;

public:
    void ChangeEditor();
    void ChangeStage();


public:
    virtual void Init() override;
    virtual void Exit() override;

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLevel_Start();
    ~CLevel_Start();
};

