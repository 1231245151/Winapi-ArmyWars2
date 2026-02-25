#pragma once
#include "CUnit.h"


class PK2Tank :
    public CUnit
{
public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void ShootBullet(Vec2 _Dir);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(PK2Tank)
    PK2Tank();
    PK2Tank(const PK2Tank& _Other);
    ~PK2Tank();
};

