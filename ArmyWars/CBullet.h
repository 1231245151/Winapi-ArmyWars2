#pragma once
#include "CObj.h"

#include "CEngine.h"
#include "CTimeMgr.h"
#include "CTexture.h"
#include "CAssetMgr.h"
#include "CCollider.h"

class CUnit;

class CBullet :
    public CObj
{
protected:
    Vec2    m_Dir;
    float   m_speed;
    float   m_Duration;
    bool    m_Hit;

    int     m_HDemage;
    int     m_MDemage;

protected:
    CCollider*  m_Body;
    CTexture*   m_Texture;      // 실제로 사용될 텍스쳐

    // 텍스쳐 정보 더 추가하기
    CTexture*   m_bulletTex1;
    CTexture*   m_bulletTex2;
    CTexture*   m_bulletTex3;
    CTexture*   m_bulletTex4;

public:
    void SetDir(Vec2 _Dir);
    void SetTexture(CTexture* _Texture) { m_Texture = _Texture; }
    void SetDuration(float _duration) { m_Duration = _duration; }
    void SetCollider(CCollider* _Collider) { m_Body = _Collider; }
    void SetSpeed(float _speed) { m_speed = _speed; }
    void SetDemage(int _H, int _M) { m_HDemage = _H; m_MDemage = _M; }



public:
    virtual void Demagecall(CUnit* _type);   //  총알이 유닛에 부딫 힐 시 함수



public:
    virtual void Tick();
    virtual void Render();
    
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) {};
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) {};

public:
    virtual CBullet* Clone() override = 0;
    CBullet();
    //CBullet(const CBullet& _Other);
    ~CBullet();
};

