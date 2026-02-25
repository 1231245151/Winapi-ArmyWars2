#pragma once
#include "CUnit.h"


class PDestroyer :
    public CUnit
{

public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void ShootBullet(Vec2 _Dir);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(PDestroyer)
    PDestroyer();
    PDestroyer(const PDestroyer& _Other);
    ~PDestroyer();
};

