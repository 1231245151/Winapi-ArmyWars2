#pragma once
#include "CUI.h"


class CTextUI :
    public CUI
{
private:
    CTexture*   m_NormalImg;
    wstring     m_Text;
    Vec2    m_TextOffset;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

public:
    void SetNormalImg(CTexture* _Atlas) { m_NormalImg = _Atlas; }
    void SetText(const wstring& _Str) { m_Text = _Str; }
    void SetTextOffset(Vec2 _Offset) { m_TextOffset = _Offset; }

public:
    CLONE(CTextUI);
    CTextUI();
    ~CTextUI();
};

