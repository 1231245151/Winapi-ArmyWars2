#pragma once
#include "CUnit.h"


class EVeteranSWAT :
    public CUnit
{


public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(EVeteranSWAT)
    EVeteranSWAT();
    EVeteranSWAT(const EVeteranSWAT& _Other);
    ~EVeteranSWAT();

};

