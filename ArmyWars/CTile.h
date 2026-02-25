#pragma once
#include "CObj.h"

class CTileMap;

class CTile :
    public CObj
{
private:
    CTileMap* m_TileMap;


public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(CTile);
    CTile();
    CTile(const CTile& _Other);
    ~CTile();
};

