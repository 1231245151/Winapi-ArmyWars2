#pragma once
#include "CUnit.h"



class EMedic :
    public CUnit
{


public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(EMedic)
    EMedic();
    EMedic(const EMedic& _Other);
    ~EMedic();
};

