#include "pch.h"
#include "CTile.h"

#include "CTileMap.h"

#include "CAssetMgr.h"

CTile::CTile()
	: m_TileMap(nullptr)
{
	m_TileMap = AddComponent(new CTileMap);
}

CTile::CTile(const CTile& _Other)
	: CObj(_Other)
	, m_TileMap(nullptr)
{
	m_TileMap = GetComponent<CTileMap>();
}


CTile::~CTile()
{
}

void CTile::Tick()
{
}

void CTile::Render()
{
	m_TileMap->Render();
}