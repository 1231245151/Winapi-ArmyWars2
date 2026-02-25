#pragma once
#include "CState.h"

class CUnit;


class EnemySommonState :
    public CState
{
private:
    float   m_Duration;
    int m_CoolCount;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

public:
    void SummonUnit(CUnit* _Unit);
    void CooltimeUnit();
    void UpgradeUnit();

public:
    CLONE_DISABLE(EnemySommonState);
    EnemySommonState();
    ~EnemySommonState();

};

