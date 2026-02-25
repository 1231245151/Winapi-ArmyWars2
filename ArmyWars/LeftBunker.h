#pragma once
#include "CBunker.h"

class CTexture;
class CCollider;

class LeftBunker :
    public CBunker
{
private:


public:
    virtual void Tick() override;
    virtual void Render() override;

public:

public:
    CLONE(LeftBunker);
    LeftBunker();
    ~LeftBunker();
};

