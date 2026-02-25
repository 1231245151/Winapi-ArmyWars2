#pragma once
#include "CUnit.h"


class EBMP2 :
    public CUnit
{
public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void ShootBullet(Vec2 _Dir);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(EBMP2)
    EBMP2();
    EBMP2(const EBMP2& _Other);
    ~EBMP2();
};

