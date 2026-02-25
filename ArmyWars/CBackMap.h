#pragma once
#include "CMap.h"


class CBackMap :
    public CMap
{
private:


public:
    virtual void Render() override;

public:
    CLONE(CBackMap)
    CBackMap();
    ~CBackMap();
};

