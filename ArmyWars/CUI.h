#pragma once
#include "CObj.h"


class CUI :
    public CObj
{
private:
    CUI* m_Parent;
    vector<CUI*>    m_vecChild;

    Vec2            m_FinalPos;

    bool            m_MouseOn;      // 현재 마우스가 UI 위에 있는지
    bool            m_MouseOn_Prev; // 이전 프레임에 마우스가 UI 위에 있었는지
    bool            m_LbtnDown;     // UI 가 마우스 왼쪽버튼이 눌린 상태인지
    bool            m_LbtnUp;     // UI 가 마우스 왼쪽버튼이 뗀 상태인지
    bool            m_Priority;     // 우선권을 가진건지 확인
    bool            m_Active;       // UI 활성화/비활성화 제어

protected:
    bool                    m_BtnOn;

public:
    CUI* GetParentUI() { return m_Parent; }
    const vector<CUI*>& GetChildUI() { return m_vecChild; }
    void DeleteChildUI(CUI* _ChildUI);
    void AddChildUI(CUI* _ChildUI)
    {
        m_vecChild.push_back(_ChildUI);
        _ChildUI->m_Parent = this;
    }

    bool GetBtnstat() { return m_BtnOn; }
    virtual Vec2 GetFinalPos();
    virtual void ActiveControl(bool _true);

    bool IsActive() { return m_Active; }
    bool IsLBtnDown() { return m_LbtnDown; }
    bool IsLBtnUp() { return m_LbtnUp; }
    bool IsMouseOn() { return m_MouseOn; }
    bool IsBtnOn() { return m_BtnOn; }
    void SetBtnstat(bool _true) { m_BtnOn = _true; }
public:
    virtual void Tick() override final;
    virtual void FinalTick() override;
    virtual void Render() override final;

    virtual void Tick_UI() = 0;
    virtual void Render_UI();

    virtual void BeginHovered() {} // 마우스가 UI 위에 막 올라와왔을 때
    virtual void OnHovered() {}   // 마우스 UI 위에 있을 때
    virtual void EndHovered() {}  // 마우스가 UI 를 빠져 나가는 순간

    virtual void LBtnDown() {}
    virtual void LBtnUp() {}
    virtual void LBtnClicked() {}

    void SetPriority(bool _Priority) { m_Priority = _Priority; }
    void SetLBtnDown(bool _true) { m_LbtnDown = _true; }

private:
    virtual void MouseOnCheck();

public:
    virtual CUI* Clone() override = 0;
    CUI();
    CUI(const CUI& _Other);
    ~CUI();

    friend class CUIMgr;
};


