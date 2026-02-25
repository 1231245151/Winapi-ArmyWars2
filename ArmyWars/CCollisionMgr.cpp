#include "pch.h"
#include "CCollisionMgr.h"

#include "CCollider.h"


CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{
}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::Init()
{
}

void CCollisionMgr::Tick()
{
	for (UINT Row = 0; Row < (int)LAYER_TYPE::END; ++Row)
	{
		for (UINT Col = Row; Col < (int)LAYER_TYPE::END; ++Col)
		{
			if (m_Matrix[Row] & (1 << Col))
			{
				if (Row != Col)
					CollisionLayer(Row, Col);
				else
					CollisionLayer(Row);
			}
		}
	}

	// 등록된 충돌체들을 전부 해제한다.
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		m_vecCollider[i].clear();
	}
}

void CCollisionMgr::CollisionCheck(UINT _Layer1, UINT _Layer2, bool _Check)
{
	// 매트릭스의 상단 부분을 쓰기위해서, Row 가 더 작은 값을 써야한다.
	UINT Row = _Layer1;
	UINT Col = _Layer2;

	if (_Layer2 < _Layer1)
	{
		Row = _Layer2;
		Col = _Layer1;
	}

	if (_Check)
	{
		m_Matrix[Row] |= (1 << Col);
	}
	else
	{
		m_Matrix[Row] &= ~(1 << Col);
	}
}



void CCollisionMgr::CollisionLayer(int Left, int Right)
{
	vector<CCollider*>& vecLeft = m_vecCollider[Left];
	vector<CCollider*>& vecRight = m_vecCollider[Right];

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			COLLIDER_ID id = {};
			id.LeftID = vecLeft[i]->GetID();
			id.RightID = vecRight[j]->GetID();

			map<DWORD_PTR, bool>::iterator iter = m_ColInfo.find(id.ID);

			// 정보가 없다 ==> 최초등록
			if (iter == m_ColInfo.end())
			{
				m_ColInfo.insert(make_pair(id.ID, false));
				iter = m_ColInfo.find(id.ID);
			}

			// 두 충돌체중 하나이상이 Dead 상태인지 확인
			bool IsDead = vecLeft[i]->GetOwner()->IsDead() || vecRight[j]->GetOwner()->IsDead();
			bool IsDeactive = vecLeft[i]->GetState() == DEACTIVE || vecRight[j]->GetState() == DEACTIVE;

			// 충돌체가 현재 충돌 중이다.
			if (Collision(vecLeft[i], vecRight[j]))
			{
				if (iter->second)
				{
					if (IsDead || IsDeactive)
					{
						// 현재 겹침, 이전에도 겹침, 둘중 하나가 곧 삭제 예정
						//vecLeft[i]->EndOverlap(vecRight[j]);
						//vecRight[j]->EndOverlap(vecLeft[i]);
						m_vecActive.push_back(ACTIVECOLLIDER{ vecLeft[i], vecRight[j], COLLIDER_TYPE::EndOverlap });
					}
					else
					{
						// 현재 겹침, 이전에도 겹침
						//vecLeft[i]->Overlap(vecRight[j]);
						//vecRight[j]->Overlap(vecLeft[i]);
						m_vecActive.push_back(ACTIVECOLLIDER{ vecLeft[i], vecRight[j], COLLIDER_TYPE::Overlap });
					}
				}
				else
				{
					if (!IsDead && !IsDeactive)
					{
						// 현재 겹침 X, 이전에는 겹침 , 둘 다 삭제예정상태 아님
						//vecLeft[i]->BeginOverlap(vecRight[j]);
						//vecRight[j]->BeginOverlap(vecLeft[i]);
						iter->second = true;
						m_vecActive.push_back(ACTIVECOLLIDER{ vecLeft[i], vecRight[j], COLLIDER_TYPE::BeginOverlap });
					}
				}
			}

			// 현재 충돌중이 아니다.
			else
			{
				// 이전에는 충돌했다.
				if (iter->second)
				{
					//vecLeft[i]->EndOverlap(vecRight[j]);
					//vecRight[j]->EndOverlap(vecLeft[i]);
					iter->second = false;
					m_vecActive.push_back(ACTIVECOLLIDER{ vecLeft[i], vecRight[j], COLLIDER_TYPE::EndOverlap });
				}
			}
		}
	}

	// 충돌체 작업 수행
	for (size_t i = 0; i < m_vecActive.size(); ++i)
	{
		switch (m_vecActive[i].Type)
		{
			case COLLIDER_TYPE::BeginOverlap:
			{
				m_vecActive[i].Left->BeginOverlap(m_vecActive[i].Right);
				m_vecActive[i].Right->BeginOverlap(m_vecActive[i].Left);
			}
			break;
			case COLLIDER_TYPE::Overlap:
			{
				m_vecActive[i].Left->Overlap(m_vecActive[i].Right);
				m_vecActive[i].Right->Overlap(m_vecActive[i].Left);
			}
			break;
			case COLLIDER_TYPE::EndOverlap:
			{
				m_vecActive[i].Left->EndOverlap(m_vecActive[i].Right);
				m_vecActive[i].Right->EndOverlap(m_vecActive[i].Left);
			}
			break;
		}
	}

	m_vecActive.clear();
}

void CCollisionMgr::CollisionLayer(int _Layer)
{
	vector<CCollider*>& vecLeft = m_vecCollider[_Layer];
	vector<CCollider*>& vecRight = m_vecCollider[_Layer];

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		for (size_t j = (i + 1); j < vecRight.size(); ++j)
		{
			COLLIDER_ID id = {};
			id.LeftID = vecLeft[i]->GetID();
			id.RightID = vecRight[j]->GetID();

			map<DWORD_PTR, bool>::iterator iter = m_ColInfo.find(id.ID);

			// 정보가 없다 ==> 최초등록
			if (iter == m_ColInfo.end())
			{
				m_ColInfo.insert(make_pair(id.ID, false));
				iter = m_ColInfo.find(id.ID);
			}

			// 두 충돌체중 하나이상이 Dead 상태인지 확인
			bool IsDead = vecLeft[i]->GetOwner()->IsDead() || vecRight[j]->GetOwner()->IsDead();
			bool IsDeactive = vecLeft[i]->GetState() == DEACTIVE || vecRight[j]->GetState() == DEACTIVE;

			// 충돌체가 현재 충돌 중이다.
			if (Collision(vecLeft[i], vecRight[j]))
			{
				if (iter->second)
				{
					if (IsDead || IsDeactive)
					{
						// 현재 겹침, 이전에도 겹침, 둘중 하나가 곧 삭제 예정
						vecLeft[i]->EndOverlap(vecRight[j]);
						vecRight[j]->EndOverlap(vecLeft[i]);
						iter->second = false;
					}
					else
					{
						// 현재 겹침, 이전에도 겹침
						vecLeft[i]->Overlap(vecRight[j]);
						vecRight[j]->Overlap(vecLeft[i]);
					}
				}
				else
				{
					if (!IsDead && !IsDeactive)
					{
						// 현재 겹침 X, 이전에는 겹침 , 둘 다 삭제예정상태 아님
						vecLeft[i]->BeginOverlap(vecRight[j]);
						vecRight[j]->BeginOverlap(vecLeft[i]);
						iter->second = true;
					}
				}
			}

			// 현재 충돌중이 아니다.
			else
			{
				// 이전에는 충돌했다.
				if (iter->second)
				{
					vecLeft[i]->EndOverlap(vecRight[j]);
					vecRight[j]->EndOverlap(vecLeft[i]);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::Collision(CCollider* _Left, CCollider* _Right)
{
	Vec2 vDiff = _Left->GetFinalPos() - _Right->GetFinalPos();

	// 두 충돌체의 각 축별 거리값
	vDiff.x = abs(vDiff.x);
	vDiff.y = abs(vDiff.y);

	// 두 충돌체의 크기를 절반씩 합친 값
	float ScaleX = _Left->GetScale().x / 2.f + _Right->GetScale().x / 2.f;
	float ScaleY = _Left->GetScale().y / 2.f + _Right->GetScale().y / 2.f;

	if (vDiff.x <= ScaleX && vDiff.y <= ScaleY)
	{
		return true;
	}

	return false;
}
