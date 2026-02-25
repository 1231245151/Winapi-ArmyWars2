#pragma once
#include "CComponent.h"



class CSearchRange :
    public CComponent
{
private:
    Vec2            m_OffsetPos;
    Vec2            m_FinalPos;
    Vec2            m_Scale;

    vector<CObj*>*         m_TargetObj;
    vector<LAYER_TYPE>    m_SearchLayer;

public:
    // 필수 설정 세팅 값
    void SetOffset(Vec2 _Offset) { m_OffsetPos = _Offset; }
    void SetOffset(float _x, float _y) { m_OffsetPos = Vec2(_x, _y); }
    void SetScale(Vec2 _Scale) { m_Scale = _Scale; }
    void SetScale(float _x, float _y) { m_Scale = Vec2(_x, _y); }
    void SetVector(vector<CObj*>* _vector) { m_TargetObj = _vector; }
    void SetSearchLayer(LAYER_TYPE _type) { m_SearchLayer.push_back(_type); }

    Vec2 GetPos();
    Vec2 GetScale() { return m_Scale; }
public:
    virtual void FinalTick() override;


public:
    CLONE(CSearchRange);
    CSearchRange(vector<CObj*>* _vector);
    CSearchRange(const CSearchRange& _Other);
    ~CSearchRange();
};

