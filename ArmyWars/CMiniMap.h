#pragma once
#include "CObj.h"

class CTexture;

class CMiniMap :
    public CObj
{
private:
    CTexture* m_BlackMap;
    CTexture* m_EnemyRed;
    CTexture* m_PlayerBlue;
    Vec2       m_UnitIconSize;



public:
    virtual void Tick();
    virtual void Render();

public:
    CLONE_DISABLE(CMiniMap)
    CMiniMap();
    ~CMiniMap();
};

