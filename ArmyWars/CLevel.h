#pragma once
#include "CBase.h"

#include "CSound.h"

class CObj;

class CLevel :
    public CBase
{
private:
    vector<CObj*>   m_arrLayer[(int)LAYER_TYPE::END];

public:
    void AddObject(CObj* _Obj, LAYER_TYPE _Type);
    vector<CObj*>& GetLayer(LAYER_TYPE _Type) { return m_arrLayer[(int)_Type]; }


    void DeleteObjects(LAYER_TYPE _Type);
    void DeleteAllObjects();

public:
    virtual void Init() = 0;
    virtual void Exit() = 0;

    virtual void Tick();
    virtual void FinalTick();
    virtual void Render();

public:
    CLONE_DISABLE(CLevel);
    CLevel();
    virtual ~CLevel();
};



