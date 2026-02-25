#pragma once
#include "CUnit.h"


class EHumvee :
    public CUnit
{

public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void ShootBullet(Vec2 _Dir);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(EHumvee)
    EHumvee();
    EHumvee(const EHumvee& _Other);
    ~EHumvee();
};

