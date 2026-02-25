#pragma once
#include "CState.h"



class DeathState :
    public CState
{
private:
    Vec2    m_OwnerPos;
    float       m_DeathDuration;    // 사망 애니메이션 지속 시간

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(DeathState);
    DeathState();
    ~DeathState();
};

