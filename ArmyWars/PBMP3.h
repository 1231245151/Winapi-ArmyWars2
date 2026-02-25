#pragma once
#include "CUnit.h"



class PBMP3 :
    public CUnit
{
public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);
    virtual void PlayDeathSound() override;
    virtual void PlayShootSound() override;

public:
    CLONE(PBMP3)
    PBMP3();
    PBMP3(const PBMP3& _Other);
    ~PBMP3();
};

