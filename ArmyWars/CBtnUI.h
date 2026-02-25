#pragma once
#include "CUI.h"

typedef void(*BTN_CALLBACK)(void);
typedef void(CBase::* BaseFunc)(void);
typedef void(CBase::* BaseFunc_Param1)(DWORD_PTR);
typedef void(CBase::* BaseFunc_Param2)(DWORD_PTR, DWORD_PTR);


class CBtnUI :
    public CUI
{
private:
    BTN_CALLBACK            m_CallBack;

    CBase*                  m_Inst;
    BaseFunc                m_MemFunc;
    BaseFunc_Param1         m_MemFunc_1;
    BaseFunc_Param2         m_MemFunc_2;

    DWORD_PTR               m_DWORD1;
    DWORD_PTR               m_DWORD2;

    CTexture*               m_NormalImg;
    Vec2                    m_NorLTopPos;
    Vec2                    m_NorScale;
    CTexture*               m_PressedImg;
    Vec2                    m_PresseLTopPos;
    Vec2                    m_PresseScale;
    CTexture*               m_OffImg;
    Vec2                    m_OffLTopPos;
    Vec2                    m_OffScale;

    bool                    m_ParentBtnOn;

public:
    void SetCallBack(BTN_CALLBACK _CallBack) { m_CallBack = _CallBack; }
    void SetDelegate(CBase* _Inst, BaseFunc _MemFunc)
    {
        m_Inst = _Inst;
        m_MemFunc = _MemFunc;
    }
    void SetDelegate1(CBase* _Inst, BaseFunc_Param1 _MemFunc, DWORD_PTR _DWORD1)
    {
        m_Inst = _Inst;
        m_MemFunc_1 = _MemFunc;
        m_DWORD1 = _DWORD1;
    }
    void SetDelegate2(CBase* _Inst, BaseFunc_Param2 _MemFunc, DWORD_PTR _DWORD1, DWORD_PTR _DWORD2)
    {
        m_Inst = _Inst;
        m_MemFunc_2 = _MemFunc;
        m_DWORD1 = _DWORD1;
        m_DWORD2 = _DWORD2;
    }
    void SetNormalImg(CTexture* _Atlas);
    void SetPressedImg(CTexture* _Atlas);
    void SetOffImg(CTexture* _Atlas);
    void SetNormalImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale);
    void SetPressedImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale);
    void SetOffImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale);

    void SetBtnstat(bool _true) { m_BtnOn = _true; }

    virtual void Tick_UI() override;
    virtual void Render_UI() override;

public:
    virtual void LBtnDown() override;
    virtual void LBtnClicked() override;

public:
    CLONE(CBtnUI);
    CBtnUI();
    ~CBtnUI();
};

