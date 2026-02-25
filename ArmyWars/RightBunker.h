#pragma once
#include "CBunker.h"


class RightBunker :
    public CBunker
{
private:


public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    virtual void HPUI();

public:
    CLONE(RightBunker);
    RightBunker();
    ~RightBunker();
};

