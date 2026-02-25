#pragma once
#include "CMap.h"



class CBackETC :
    public CMap
{
private:

public:
    virtual void Render() override;

public:
    CLONE(CBackETC)
    CBackETC();
    ~CBackETC();

};

