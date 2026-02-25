#pragma once
#include "CBullet.h"


class CTankBullet :
    public CBullet
{
public:
    virtual void Demagecall(CUnit* _type);

public:
    CLONE(CTankBullet)
    CTankBullet();
    ~CTankBullet();
};

