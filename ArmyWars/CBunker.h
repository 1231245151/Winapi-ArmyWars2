#pragma once
#include "CObj.h"



class CTexture;
class CStateMachine;
class CSearchRange;
class CCollider;


class CBunker :
    public CObj
{
protected:
    Unit_Stat           m_BaseStat;
    CTexture*           m_Basetex;
    CCollider*          m_Body;
    CSearchRange*       m_Range;
    CStateMachine*      m_State;
    CStateMachine*      m_SummonState;
    vector<CObj*>         m_TargetObj;
    Vec2                m_BodyOffset;

    int     m_MaxFund;
    int     m_HaveFund;
    int     m_FUNDHeal;
    float   m_FUNDDuration;
    float   m_FUNDRate;

    CTexture*           m_Defencetex;

public:
    void SetTexture(CTexture* _base) { m_Basetex = _base; }
    void SetDefenceTex(CTexture* _Defence) { m_Defencetex = _Defence; }
    void FlusFUND(int _FUND);

    CTexture* GetTexture() { return m_Basetex; }
    Unit_Stat& GetBaseStat() { return m_BaseStat; }
    vector<CObj*>& GetTargetObj(){return m_TargetObj;}
    int& GetHaveFUND() { return m_HaveFund; }
    int& GetMaxFUND() { return m_MaxFund; }
    Vec2 GetBodyPos();
    Vec2 GetBodyScale();

public:
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);     // 공격을 받았을 시 체력 계산, 가장 강한 공격력을 참조해 계산함

    virtual void HPUI();
    virtual void SetSummonState();
    virtual void DeleteSummonState();

public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) {};
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    virtual CBunker* Clone() override = 0;
    CBunker();
    ~CBunker();
};

