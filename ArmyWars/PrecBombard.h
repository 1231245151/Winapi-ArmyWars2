#pragma once
#include "CSkill.h"

class CSearchRange;

class PrecBombard :
    public CSkill
{
private:
    Vec2                m_Velocity;
    CSearchRange*       m_Range;      // 범위 설정
    vector<CObj*>       m_TargetObj;

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void ActiveSkill() override;

public:
    CLONE(PrecBombard);
    PrecBombard();
    PrecBombard(const PrecBombard& _Other);
    ~PrecBombard();

};

