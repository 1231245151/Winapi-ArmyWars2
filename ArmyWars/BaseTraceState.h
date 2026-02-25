#pragma once
#include "CState.h"



class BaseTraceState :
    public CState
{
private:
    Vec2    m_OwnerPos;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BaseTraceState);
    BaseTraceState();
    ~BaseTraceState();
};
