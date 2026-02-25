#pragma once
#include "CObj.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CAnimator.h"

class CAnimator;

class CEffect :
    public CObj
{
protected:
    CAnimator* m_Animation;
    CObj* m_Owner;
    Vec2  m_Offset;


public:
    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; SetPos(GetPos() + _Offset); }
    void SetOwner(CObj* _Owner) { m_Owner = _Owner; }


public:
    virtual void Tick();
    virtual void Render();


public:
    CLONE_DISABLE(CEffect)
    CEffect();
    ~CEffect();
};

