#pragma once
#include "CComponent.h"


class CState;

class CStateMachine :
    public CComponent
{
private:
    map<wstring, CState*>   m_mapState;
    CState* m_CurState;


public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    void ChangeState(const wstring& _StateName);

    const wstring& PresentStateName();

public:
    virtual void FinalTick() override;

public:
    CLONE(CStateMachine);
    CStateMachine();
    CStateMachine(const CStateMachine& _Other);
    ~CStateMachine();
};

