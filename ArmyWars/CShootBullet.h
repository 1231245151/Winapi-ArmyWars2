#pragma once
#include "CEffect.h"



class CShootBullet :
    public CEffect
{
private:
    float m_Duration;

public:
    virtual void Tick();

public:
    CShootBullet();
    ~CShootBullet();

};

