#pragma once
#include "CBullet.h"



class CBazukaBullet :
    public CBullet
{
public:
    virtual void Demagecall(CUnit* _type);
public:
    CLONE(CBazukaBullet)
    CBazukaBullet();
    ~CBazukaBullet();
};

