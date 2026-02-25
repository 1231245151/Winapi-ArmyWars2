#pragma once
#include "CUnit.h"


class ESniper :
    public CUnit
{
private:


public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void ShootBullet(Vec2 _Dir);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(ESniper)
    ESniper();
    ESniper(const ESniper& _Other);
    ~ESniper();
};

