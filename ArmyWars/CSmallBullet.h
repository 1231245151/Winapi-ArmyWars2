#pragma once
#include "CBullet.h"




class CSmallBullet :
    public CBullet
{
public:
    virtual void Demagecall(CUnit* _type);

public:
    CLONE(CSmallBullet)
    CSmallBullet();
    ~CSmallBullet();
};

