#pragma once
#include "CSkill.h"


class CRepair :
    public CSkill
{
private:


public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void ActiveSkill() override;


public:
    CLONE(CRepair)
    CRepair();
    ~CRepair();

};

