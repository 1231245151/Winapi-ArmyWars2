#pragma once
#include "CBullet.h"

class CAnimator;

class CAirBullet :
    public CBullet
{
private:
    Vec2 m_TargetPos;
    Vec2 m_Velocity;
    bool m_Hit;
    bool m_EffectAnimOn;

    CAnimator* m_EffectAnim;

public:
    void SetTargetPos(Vec2 _Target) { m_TargetPos = _Target; }

public:
    virtual void Tick() override;
    virtual void Render() override;
    virtual void Demagecall(CUnit* _type) override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CAirBullet)
    CAirBullet();
    ~CAirBullet();
};

