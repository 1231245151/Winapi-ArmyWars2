#include "pch.h"
#include "CUnitMgr.h"

#include "CCamera.h"

#include "CUnit.h"
#include "PSolider.h"
#include "PSWAT.h"
#include "PSniper.h"
#include "PDestroyer.h"
#include "PHumvee.h"
#include "PMedic.h"
#include "PScout.h"
#include "PHeavySolider.h"
#include "PVeteranSWAT.h"
#include "PVeteranHeavy.h"
#include "PBMP3.h"
#include "PVeteranSolider.h"
#include "PK2Tank.h"

#include "ESolider.h"
#include "ESWAT.h"
#include "ESniper.h"
#include "EMedic.h"
#include "EScout.h"
#include "EHeavySolider.h"
#include "EDestroyer.h"
#include "ETruck.h"
#include "EVeteranSolider.h"
#include "EVeteranSWAT.h"
#include "EHumvee.h"
#include "EBMP2.h"
#include "EBradly.h"
#include "EK2Tank.h"

#include "CRepair.h"
#include "CAirSupport.h"
#include "PrecBombard.h"


CUnitMgr::CUnitMgr()
{

}

CUnitMgr::~CUnitMgr()
{
	for (int i = 0; i < (int)PLAYER_UNIT_NAME::END; ++i)
	{
		delete m_PUnitVec[i];
		m_PUnitVec[i] = nullptr;
	}

	for (int i = 0; i < (int)ENEMY_UNIT_NAME::END; ++i)
	{
		delete m_EUnitVec[i];
		m_EUnitVec[i] = nullptr;
	}

	for (int i = 0; i < (int)SKILL_NAME::END; ++i)
	{
		delete m_SkillVec[i];
		m_SkillVec[i] = nullptr;
	}

}

void CUnitMgr::Init()
{
	// 사용될 유닛, 스킬들의 정보 저장
	PUnitInit();
	EUnitInit();
	SkillInit();

	m_CooltimeTex = CAssetMgr::Get()->CreateTexture(L"Coolwhite", 90, 90);
	SELECT_BRUSH(m_CooltimeTex->GetDC(), BRUSH_TYPE::WHITE);
	Rectangle(m_CooltimeTex->GetDC(), -1, -1, (int)m_CooltimeTex->GetWidth() + 1, (int)m_CooltimeTex->GetHeight() + 1);

	// 소환될 유닛의 정보 저장
	m_PSummonIndx.push_back(m_PUnitVec[(int)PLAYER_UNIT_NAME::SOLIDER]);
	m_PSummonIndx.push_back(m_PUnitVec[(int)PLAYER_UNIT_NAME::SWAT]);
	m_PSummonIndx.push_back(m_PUnitVec[(int)PLAYER_UNIT_NAME::SNIPER]);
	m_PSummonIndx.push_back(m_PUnitVec[(int)PLAYER_UNIT_NAME::MEDIC]);
	m_PSummonIndx.push_back(m_PUnitVec[(int)PLAYER_UNIT_NAME::HUMVEE]);


	m_ESummonIndx.push_back(m_EUnitVec[(int)ENEMY_UNIT_NAME::SOLIDER]);
}

void CUnitMgr::Tick()
{
	// 해당 기능은 스테이지 상태일 경우에만 돌아가도록 한다
	if ((CCamera::Get()->GetCameraStat() & STAGECAMERAMODE) && (0 != DT))
	{
		// 생산 쿨타임 계산
		for (int i = 0; i < m_PSummonIndx.size(); ++i)
		{
			Unit_Stat& fUnit = m_PSummonIndx[i]->GetStat();
			// 0초과일 시 계산
			if (fUnit.CooltimeDuration > 0.f)
			{
				fUnit.CooltimeDuration -= DT;
				// 쿨타임이 0미만이면 0으로 고정
				if (fUnit.CooltimeDuration < 0.f)
					fUnit.CooltimeDuration = 0.f;
			}
		}

		// 적 유닛 쿨타임 계산
		for (int i = 0; i < m_ESummonIndx.size(); ++i)
		{
			Unit_Stat& fUnit = m_ESummonIndx[i]->GetStat();
			// 0초과일 시 계산
			if (fUnit.CooltimeDuration > 0.f)
			{
				fUnit.CooltimeDuration -= DT;
				// 쿨타임이 0미만이면 0으로 고정
				if (fUnit.CooltimeDuration < 0.f)
					fUnit.CooltimeDuration = 0.f;
			}

		}

		// 스킬 쿨타임 계산
		for (int i = 0; i < (int)SKILL_NAME::END; ++i)
		{
			if (nullptr == m_SkillVec[i])
				break;

			Unit_Stat& SkillStat = m_SkillVec[i]->GetStat();
			// 0초과일 시 계산
			if (SkillStat.CooltimeDuration > 0.f)
			{
				SkillStat.CooltimeDuration -= DT;
				// 쿨타임이 0미만이면 0으로 고정
				if (SkillStat.CooltimeDuration < 0.f)
					SkillStat.CooltimeDuration = 0.f;
			}
		}
	}
}

void CUnitMgr::PUnitInit()
{
	// 플레이어 유닛 정보 초기화
	m_PUnitVec[(int)PLAYER_UNIT_NAME::SOLIDER] = new PSolider;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::SWAT] = new PSWAT;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::SNIPER] = new PSniper;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::MEDIC] = new PMedic;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::SCOUT] = new PScout;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::DESTROYER] = new PDestroyer;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::HEAVYSOLIDR] = new PHeavySolider;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::VETERANSOLIDER] = new PVeteranSolider;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::VETERANSWAT] = new PVeteranSWAT;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::VETERANHEAVY] = new PVeteranHeavy;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::HUMVEE] = new PHumvee;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::IFV] = new PBMP3;
	m_PUnitVec[(int)PLAYER_UNIT_NAME::K2TANK] = new PK2Tank;
}

void CUnitMgr::EUnitInit()
{
	// 적 유닛 정보 초기화
	m_EUnitVec[(int)ENEMY_UNIT_NAME::SOLIDER] = new ESolider;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::SWAT] = new ESWAT;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::SNIPER] = new ESniper;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::MEDIC] = new EMedic;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::SCOUT] = new EScout;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::HEAVYSOLIDR] = new EHeavySolider;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::DESTROYER] = new EDestroyer;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::HUMVEE] = new EHumvee;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::VETERANSOLIDER] = new EVeteranSolider;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::VETERANSWAT] = new EVeteranSWAT;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::TRUCK] = new ETruck;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::BRADLY] = new EBradly;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::IFV] = new EBMP2;
	m_EUnitVec[(int)ENEMY_UNIT_NAME::K2TANK] = new EK2Tank;
}

void CUnitMgr::SkillInit()
{
	m_SkillVec[(int)SKILL_NAME::REPAIR] = new CRepair;
	m_SkillVec[(int)SKILL_NAME::AIRSTRIKE] = new CAirSupport;
	m_SkillVec[(int)SKILL_NAME::PRECISIONBOOM] = new PrecBombard;
}

void CUnitMgr::SetPSummonIndx(PLAYER_UNIT_NAME _Name)
{
	m_PSummonIndx.push_back(m_PUnitVec[(int)_Name]);
}

void CUnitMgr::DeletePSummonIndx(PLAYER_UNIT_NAME _Name)
{
	vector<CUnit*>::iterator iter = m_PSummonIndx.begin();

	for (; iter != m_PSummonIndx.end(); ++iter)
	{
		if ((*iter)->GetPUnitName() == _Name)
		{
			// 해당 데이터 지우고 끝내기
			m_PSummonIndx.erase(iter);
			return;
		}
	}
	// 여기에 도달한다면 지울 유닛을 못찾은 것
	LOG(LOG_LEVEL::WARNING, L"제대로 유닛 목록이 지워지지 않았습니다.")
}

void CUnitMgr::SetESummonIndx(ENEMY_UNIT_NAME _Name)
{
	m_ESummonIndx.push_back(m_EUnitVec[(int)_Name]);
}

void CUnitMgr::ClearESummonIndx()
{
	m_ESummonIndx.clear();

	// 초기화가 진행된다면 킬 리스트도 필요없어진것
	std::fill(m_PKillCountList, m_PKillCountList + (int)PLAYER_UNIT_NAME::END, 0);
	std::fill(m_EKillCountList, m_EKillCountList + (int)ENEMY_UNIT_NAME::END, 0);
}

vector<CUnit*>& CUnitMgr::GetPSummonIndx()
{
	return m_PSummonIndx;
}

vector<CUnit*>& CUnitMgr::GetESummonIndx()
{
	return m_ESummonIndx;
}


CUnit* CUnitMgr::SearchPUnitByName(PLAYER_UNIT_NAME _Name)
{
	//찾기
	for (int i = 0; i < m_PSummonIndx.size(); ++i)
	{
		if (_Name == m_PSummonIndx[i]->GetPUnitName())
			return m_PSummonIndx[i];
	}


	//찾지 못하면 nullptr반환
	return nullptr;
}