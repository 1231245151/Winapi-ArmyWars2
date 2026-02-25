#pragma once
#include "CLevel_Stage.h"
class CLevel_Test :
    public CLevel_Stage
{


private:
    virtual void Init() override;

public:
    CLevel_Test();
    ~CLevel_Test();
};

