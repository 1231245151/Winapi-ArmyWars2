#pragma once
#include "CBase.h"
#include "CAnimator.h"

struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    float   Duration;
};

class CTexture;

class CAnim :
    public CBase
{
private:
    CAnimator* m_Owner;
    Vec2        m_Offset;
    vector<tAnimFrm>    m_vecFrm;
    CTexture* m_Atlas;
    int                 m_CurIdx;
    float               m_AccTime;
    bool                m_Finish;

public:
    void Create(const tAnimDesc& _Info);
    bool IsFinish() { return m_Finish; }
    void Reset()
    {
        m_Finish = false;
        m_AccTime = 0.f;
        m_CurIdx = 0;
    }

    tAnimFrm& GetFrame(int _Idx) { return m_vecFrm[_Idx]; }

public:
    void FinalTick();
    void Render();

    void Save(const wstring& _FolderPath);
    void Load(const wstring& _FilePath);
    void SetOffset(Vec2 _Offset) { m_Offset = _Offset; }

public:
    CLONE(CAnim);
    CAnim();
    CAnim(const CAnim& _Other);
    ~CAnim();

    friend class CAnimator;
};

