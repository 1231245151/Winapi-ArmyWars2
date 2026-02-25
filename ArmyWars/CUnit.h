#pragma once
#include "CObj.h"
#include "CBullet.h"
#include "CSmallBullet.h"
#include "CStateMachine.h"
#include "CAnimator.h" 
#include "CPathMgr.h"
#include "CSearchRange.h"
#include "CShootBullet.h"
#include "CShootBazuka.h"
#include "CShootShotgun.h"
#include "CHitBullet.h"
#include "CTankBullet.h"


#include "TraceState.h"
#include "ShootState.h"
#include "DeathState.h"

//Type : 유닛의 종류입니다.
//FUND : 소환 시 필요한 유닛 코스트입니다.
//MaxHP : 유닛의 최대 체력입니다. 0이하가 되면 사망합니다.
//HDemage : 보병형 유닛에게 주는 피해입니다.
//MDemage : 차량 유닛에게 주는 피해입니다.
//Armor : 피해가 들어오면 해당 수치만큼 차감되어 HP피해를 입습니다.
//AttackSpeed : 다음 발사 까지 걸리는 시간입니다.낮을 수록 공격 속도가 빠릅니다.
//Range : 유닛의 사거리입니다.공격 범위 내에 가까운 유닛이나 기지를 포착하면 총을 발사합니다.
//velocity : 이동속도입니다.높을수록 빠릅니다.
//Cooltime : 생산 쿨타임입니다.낮을 수록 다음 생산까지 걸리는 시간이 짧습니다.

class CSearchRange;

class CUnit :
    public CObj
{
    // 자식 클래스에서 설정해야하는 정보들
private:
    Unit_Stat   m_Stat;         // 유닛의 기본 스텟입니다.
    UNIT_ATST   m_AtMoveType;	// 공격 시 이동 방식 입니다.
    UNIT_ATTACK m_ShootInst;    // 
    bool        m_MoveStop;     // 이동정지용 명령어
    bool        m_Death;        // 사망상태

protected:
    CAnimator*          m_Animator;
    CCollider*          m_Body;
    CStateMachine*      m_State;
    CSearchRange*       m_Range;
    vector<CObj*>       m_TargetObj;

    PLAYER_UNIT_NAME    m_PUnitName;
    ENEMY_UNIT_NAME     m_EUnitName;

    // 총알, 몸위치
    Vec2                m_BulletStart;
    Vec2                m_BulletOffset;
    Vec2                m_BodyOffset;
    CTexture*           m_RangeTexture;

    // 아이콘 설정
    CTexture*           m_IconTexture1;
    CTexture*           m_IconTexture2;
    Vec2                m_IconLPos;
    Vec2                m_IconScale;

public:
    Unit_Stat& GetStat() { return m_Stat; }
    CCollider* GetBody() { return m_Body; }
    CAnimator* GetAinmator() { return m_Animator; }
    bool GetMovestat() { return m_MoveStop; }
    Vec2 GetBodyPos() { return (GetPos() + m_BodyOffset); }
    Vec2 GetBodyOffset() { return m_BodyOffset; }
    Vec2 GetBulletStart() { return m_BulletStart; }
    Vec2 GetBulletOffset() { return m_BulletOffset; }
    Vec2 GetBulletShootPos() { return (GetPos() + m_BulletOffset); }

    PLAYER_UNIT_NAME GetPUnitName() { return m_PUnitName; }
    ENEMY_UNIT_NAME GetEUnitName() { return m_EUnitName; }

    CTexture* GetRangeTexture() { return m_RangeTexture; }
    CTexture* GetIconTexture1() { return m_IconTexture1; }
    CTexture* GetIconTexture2() { return m_IconTexture2; }
    Vec2      GetIconSize() { return m_IconScale; }
    Vec2      GetIconLPos() { return m_IconLPos; }

    vector<CObj*>& GetTargetObj() { return m_TargetObj; }

    UNIT_ATTACK GetShootType() { return m_ShootInst; }

    void SetMove(bool _true) { m_MoveStop = _true; }
    void SetShootType(UNIT_ATTACK _true) { m_ShootInst = _true; }
    void SetAnimator(CAnimator* _Anim) { m_Animator = _Anim; }
    void SetBody (CCollider* _Collider) { m_Body = _Collider; }

public:
    virtual void PlayAnimation(const wstring& _Name, bool _Repeat) = 0;
    virtual void PlayDeathSound() = 0;
    virtual void PlayShootSound() = 0;
    virtual void ShootBullet(Vec2 _velocity);
    virtual void Demagecall(Unit_Stat& _EnemyStat, CObj* _Other);     // 공격을 받았을 시 체력 계산

    virtual void HPUI();

public:
    virtual void Tick();
    virtual void Render();

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    virtual CUnit* Clone() override = 0;
    CUnit();
    CUnit(const CUnit& _Other);
    ~CUnit();
};

