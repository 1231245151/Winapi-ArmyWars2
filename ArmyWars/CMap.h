#pragma once
#include "CObj.h"


class CMap :
    public CObj
{
protected:
    MapInfo		m_MapInfo;
    float        m_PosRate;



public:
    CTexture* GetTexture() { return m_MapInfo.MapTexture; }
    float GetLegth();
    UINT  GetPlatCol() { return m_MapInfo.PlatCol; }
    int GetIndx() { return m_MapInfo.MapIndx; }

    int SetIndx(int _Indx);
    void SetPlatCol(UINT _Col) { m_MapInfo.PlatCol = _Col; }
    void SetPosRate(float _rate) { m_PosRate = _rate; }

public:
    virtual void Tick() override {};
    virtual void Render() override;

public:
    virtual CObj* Clone() = 0;
    CMap();
    ~CMap();

};

