#pragma once
#include "CLevel.h"


class CMap;
class CTexture;
class LeftBunker;
class RightBunker;



class CLevel_Editor
	: public CLevel
{
private:
	HMENU		m_hMenu;

	CMap*		m_BackETC;
	CMap*		m_BackMap;
	CMap*		m_PlatMap;

	Vec2		m_MapPosBack;
	Vec2		m_MapPosBackE;
	Vec2		m_MapPlat;

	LeftBunker*		pBunker;
	RightBunker*	eBunker;			

	Vec2			m_pBunkerPos;		// 왼쪽 벙커 위치
	Vec2			m_eBunkerPos;		// 오른쪽 벙커 위치

	UINT	m_MapCol;		// 맵 개수 선택
	int		m_MapIndx;		// 맵 디자인 선택

	float	m_LineLegth;		// 베이스와 베이스 기준 거리며 유닛의 이동될 기준
	Vec2	m_CameraLook;		// 시작 시 카메라 위치

public:
	int	GetMapIndx() { return m_MapIndx; }
	float	GetLegth() { return m_LineLegth; }

	Vec2& GetCameraLook() { return m_CameraLook; }

	void SaveMap();
	void LoadMap();
	void TileSet(float _Legth);
	void MapChange(int _info);

private:
	virtual void Init() override;
	virtual void Exit() override;
	virtual void Tick() override;

public:
	CLevel_Editor();
	~CLevel_Editor();
};