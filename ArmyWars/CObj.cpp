#include "pch.h"
#include "CObj.h"

#include "CComponent.h"
#include "CCamera.h"
#include "CEngine.h"


CObj::CObj()
	: m_LayerType(-1)
	, m_bDead(false)
{
}

CObj::CObj(const CObj& _Other)
	: CBase(_Other)
	, m_Pos(_Other.m_Pos)
	, m_Scale(_Other.m_Scale)
	, m_LayerType(-1)
	, m_bDead(false)
{
	for (size_t i = 0; i < _Other.m_vecComponent.size(); ++i)
	{
		CComponent* pClonedComponent = _Other.m_vecComponent[i]->Clone();
		AddComponent(pClonedComponent);
	}
}

CObj::~CObj()
{
	Release_Vector(m_vecComponent);
}


Vec2 CObj::GetRenderPos()
{
	return CCamera::Get()->GetRenderPos(m_Pos);
}

Vec2 CObj::GetTileRenderPos()
{
	return CCamera::Get()->GetRenderPos(m_Pos);
}

CComponent* CObj::GetComponent(COMPONENT_TYPE _Type)
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		if (m_vecComponent[i]->GetComponentType() == _Type)
			return m_vecComponent[i];
	}

	return nullptr;
}

CComponent* CObj::GetComponentByName(const wstring& _Name)
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		if (m_vecComponent[i]->GetName() == _Name)
		{
			return m_vecComponent[i];
		}
	}

	return nullptr;
}

void CObj::FinalTick()
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		m_vecComponent[i]->FinalTick();
	}
}

void CObj::Render()
{
	HDC dc = CEngine::Get()->GetBackDC();

	SELECT_PEN(dc, PEN_TYPE::RED);
	SELECT_BRUSH(dc, BRUSH_TYPE::BLUE);


	Vec2 vRenderPos = CCamera::Get()->GetRenderPos(m_Pos);

	Rectangle(dc, vRenderPos.x - (m_Scale.x / 2.f), vRenderPos.y - (m_Scale.y / 2.f)
		, vRenderPos.x + (m_Scale.x / 2.f), vRenderPos.y + (m_Scale.y / 2.f));
}