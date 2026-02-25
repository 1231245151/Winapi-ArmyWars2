#pragma once
#include "CObj.h"

#include "CEngine.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"

#include "CAnimator.h"

class CSkill :
    public CObj
{
protected:
    Unit_Stat   m_SkillStat;
    float       m_Duration;         // 유지 시간, 끝나면 삭제
    bool        m_Active;           // 사용되는 스킬인지 확인

    // 애니메이션 설정
    CAnimator* m_Animator;

    // 아이콘 설정
    CTexture*           m_IconTexture1;
    CTexture*           m_IconTexture2;
    Vec2                m_IconLPos;
    Vec2                m_IconScale;

    CAnimator* m_DomiAnim;

    // 특정 스킬 사거리 설정
    CTexture* m_RangeTexture;

public:
    // 스텟 정보
    Unit_Stat& GetStat() { return m_SkillStat; }

    // 아이콘 관련 텍스쳐 정보
    CTexture* GetRangeTexture() { return m_RangeTexture; }
    CTexture* GetIconTexture1() { return m_IconTexture1; }
    CTexture* GetIconTexture2() { return m_IconTexture2; }
    Vec2      GetIconSize() { return m_IconScale; }
    Vec2      GetIconLPos() { return m_IconLPos; }

    CAnimator* GetDomiAnim() { return m_DomiAnim; }
public:
    virtual void Tick();
    virtual void Render();

    // 스킬사용 함수
    virtual void ActiveSkill() = 0;


public:
    CSkill();
    CSkill(const CSkill& _Other);
    ~CSkill();
};

