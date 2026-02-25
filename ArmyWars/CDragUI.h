#pragma once
#include "CUI.h"


typedef void(*BTN_CALLBACK)(void);
typedef void(CBase::* BaseFunc)(void);
typedef void(CBase::* BaseFunc_Param1)(DWORD_PTR);
typedef void(CBase::* BaseFunc_Param2)(DWORD_PTR, DWORD_PTR);

class CAnimator;


class CDragUI :
    public CUI
{
private:
    // 함수 포인터
    BTN_CALLBACK            m_CallBack;

    CBase* m_Inst;
    BaseFunc                m_MemFunc;
    BaseFunc_Param1         m_MemFunc_1;
    BaseFunc_Param2         m_MemFunc_2;

    DWORD_PTR               m_DWORD1;
    DWORD_PTR               m_DWORD2;

    // 드래그 위치
    Vec2    m_GrabPos;
    Vec2    m_OriginalPos;

    // 텍스쳐정보
    CTexture*               m_NormalImg;
    Vec2                    m_NorLTopPos;
    Vec2                    m_NorScale;
    Vec2                    m_NorOffSetPos;
    Vec2                    m_NorRenderScale;
    UINT                    m_NorAlpha;
    CTexture*               m_PressedImg;
    Vec2                    m_PresseLTopPos;
    Vec2                    m_PresseScale;
    Vec2                    m_PresseOffSetPos;
    Vec2                    m_PresseRenderScale;
    UINT                    m_PresseAlpha;
    CTexture*               m_OffImg;
    Vec2                    m_OffLTopPos;
    Vec2                    m_OffScale;
    Vec2                    m_OffOffSetPos;
    Vec2                    m_OffRenderScale;
    UINT                    m_OffAlpha;

    // 애니메이션 정보
    CAnimator*              m_Animation;

    bool                    m_DragOn;                   // 드래그 상태임을 확인

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
    void SetAnimation(CAnimator* _Animator);

    void SetNormalImg(CTexture* _Atlas, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f, 0.f));
    void SetPressedImg(CTexture* _Atlas, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f, 0.f));
    void SetOffImg(CTexture* _Atlas, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f, 0.f));
    void SetNormalImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f, 0.f));
    void SetPressedImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f,0.f));
    void SetOffImg(CTexture* _Atlas, Vec2 _LTopPos, Vec2 _Scale, Vec2 _Offset = Vec2(0.f, 0.f), UINT _Alpha = 255, Vec2 _RenderScale = Vec2(0.f, 0.f));

    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    void RenderTexture();


public:
    virtual void LBtnDown() override;
    virtual void LBtnUp() override;
    virtual void LBtnClicked() override;

public:
    CLONE(CDragUI);
    CDragUI();
    ~CDragUI();
};


