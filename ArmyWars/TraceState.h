#pragma once
#include "CState.h"


class TraceState :
    public CState
{
private:
    Vec2    m_OwnerPos;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(TraceState);
    TraceState();
    ~TraceState();
};

