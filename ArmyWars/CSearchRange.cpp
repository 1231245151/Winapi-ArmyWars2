#include "pch.h"
#include "CSearchRange.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CCollider.h"

CSearchRange::CSearchRange(vector<CObj*>* _vector)
	:CComponent(COMPONENT_TYPE::SEARCH_RANGE)
	, m_TargetObj(_vector)
{
}

CSearchRange::CSearchRange(const CSearchRange& _Other)
	:CComponent(COMPONENT_TYPE::SEARCH_RANGE)
	, m_OffsetPos(_Other.m_OffsetPos)
	, m_FinalPos(_Other.m_OffsetPos)
	, m_Scale(_Other.m_Scale)
	, m_TargetObj(_Other.m_TargetObj)
	, m_SearchLayer(_Other.m_SearchLayer)
{
}

CSearchRange::~CSearchRange()
{
}

Vec2 CSearchRange::GetPos()
{
	return GetOwner()->GetPos() + m_OffsetPos;
}

void CSearchRange::FinalTick()
{
	// 검사 전 등록된 타겟들은 초기화 시킨다.
	m_TargetObj->clear();

	// 최종 위치 확인
	m_FinalPos = GetOwner()->GetPos() + m_OffsetPos;


	// 레벨에 등록된 레이어들 탐색
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// 찾기를 원하는 레이어들을 검색
	for (int j = 0; j < (int)m_SearchLayer.size(); ++j)
	{
		vector<CObj*>& CurVec = CurLevel->GetLayer(m_SearchLayer[j]);

		for (int i = 0; i < CurVec.size(); ++i)
		{
			// 탐색 대상은 충돌체 컴포넌트가 있어야 검사가 가능함
			CCollider* CurCol = CurVec[i]->GetComponent<CCollider>();
			if (nullptr == CurCol)
			{
				continue;
			}
			Vec2 EnemyScale = CurCol->GetScale();
			Vec2 EnemyPos = CurCol->GetFinalPos();

			// 사거리와 대상체의 거리값
			Vec2 vDiff = m_FinalPos - EnemyPos;
			vDiff.x = abs(vDiff.x);
			vDiff.y = abs(vDiff.y);

			// 감지 범위
			float ScaleX = m_Scale.x / 2.f + EnemyScale.x / 2.f;
			float ScaleY = m_Scale.y / 2.f + EnemyScale.y / 2.f;

			// 사거리 안이라면 백터에 등록한다.
			if (vDiff.x <= ScaleX && vDiff.y <= ScaleY)
			{
				m_TargetObj->push_back(CurVec[i]);
			}

			CurCol = nullptr;
		}
	}

	// 디버그 랜더링
	Vec2 vRenderPos = CCamera::Get()->GetRenderPos(m_FinalPos);
	if (0 < m_TargetObj->size())
		DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::RED, BRUSH_TYPE::HOLLOW, vRenderPos, m_Scale);
	else
		DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vRenderPos, m_Scale);
}