#pragma once
#include "CSkill.h"

class CSearchRange;


class CAirSupport :
    public CSkill
{
private:
    Vec2 m_Velocity;
    CSearchRange* m_Range;
    vector<CObj*>       m_TargetObj;
    bool                m_IsLeftGun;

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void ActiveSkill() override;
public:
    CLONE(CAirSupport);
    CAirSupport();
    CAirSupport(const CAirSupport& _Other);
    ~CAirSupport();
};

