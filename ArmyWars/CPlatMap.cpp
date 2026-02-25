#include "pch.h"
#include "CPlatMap.h"

#include "CAssetMgr.h"
#include "CEngine.h"

CPlatMap::CPlatMap()
{
	m_MapInfo.MapTexture = CAssetMgr::Get()->LoadTexture(L"PlatTex", L"texture\\stage\\mapField2.png");

	SetScale(768.f, 360.f);
	SetPos(0.f, 600.f);
}

CPlatMap::~CPlatMap()
{
}


void CPlatMap::Render()
{
	CMap::Render();
}