#pragma once
#include "CUI.h"

class CHoldUI :
    public CUI
{
private:
    CTexture* m_NormalImg;
    Vec2 RenderPos;
    Vec2 RenderScale;


public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

public:
    virtual void LBtnDown();
    void SetNormalImg(CTexture* _Atlas, Vec2 _RenderPos = Vec2(0.f, 0.f), Vec2 _RenderScale = Vec2(0.f, 0.f));

public:
    CLONE(CHoldUI);
    CHoldUI();
    ~CHoldUI();
};
