#pragma once
#include "CMap.h"


class CPlatMap :
    public CMap
{
protected:

public:
    virtual void Render() override;

public:
    CLONE(CPlatMap)
    CPlatMap();
    ~CPlatMap();
};

