#include "pch.h"
#include "EnemySommonState.h"
#include "CTimeMgr.h"

#include "CUnit.h"
#include "CUnitMgr.h"
#include "CBunker.h"
#include "CLevelMgr.h"
#include "CLevel_Stage.h"
#include "CLogMgr.h"


EnemySommonState::EnemySommonState()
	:m_Duration(0.f)
	, m_CoolCount(0)
{
}

EnemySommonState::~EnemySommonState()
{
}

void EnemySommonState::Enter()
{
	// 유닛 정보 목록
	vector<CUnit*>& EUnitIndx = CUnitMgr::Get()->GetESummonIndx();
	// 유닛 초기 쿨타임 설정
	// 설정된 초기 쿨타임 + 기본 쿨타임의 0~1.5배
	for(int i = 0; i < EUnitIndx.size(); ++i)
	{
		random_device rd;           // 하드웨어 기반의 랜덤 시드 생성
		mt19937 gen(rd());          // 메르센 트위스트 난수 생성을 rd시드로 초기화
		uniform_real_distribution<> dis(0, 1.5); // 0부터 1.5까지 균등 분포된 실수 생성

		float random_number = dis(gen);    // gen을 사용하여 1부터 1.5까지의 랜덤 실수 생성

		Unit_Stat& CurUnitStat = EUnitIndx[i]->GetStat();

		CurUnitStat.CooltimeDuration = CurUnitStat.firstCooltime + (random_number * CurUnitStat.CooltimeOrgin);


		wstring str2 = std::to_wstring(CurUnitStat.CooltimeDuration);
		LOG(LOG_LEVEL::LOG, str2.c_str())
	}

}

void EnemySommonState::FinalTick()
{
	// 유닛 쿨타임 확인
	CooltimeUnit();

	// 적유닛 시간이 지나면 쿨타임 감소
	m_Duration += DT;
	UpgradeUnit();

}

void EnemySommonState::Exit()
{

}


void EnemySommonState::CooltimeUnit()
{
	// 유닛 정보 목록
	vector<CUnit*>& EUnitIndx = CUnitMgr::Get()->GetESummonIndx();

	// 목록으로 반복문 돌리기
	for (int i = 0; i < EUnitIndx.size(); ++i)
	{
		if (0.f >= EUnitIndx[i]->GetStat().CooltimeDuration)
		{
			// 쿨타임이 종료된 유닛들은 소환개시
				SummonUnit(EUnitIndx[i]);
		}

	}
}

void EnemySommonState::UpgradeUnit()
{
	// 유닛 정보 목록
	vector<CUnit*>& EUnitIndx = CUnitMgr::Get()->GetESummonIndx();
	// 시간이 지남에 따라 쿨타임 감소
	if (m_Duration > 60.f && m_CoolCount == 0)
	{
		for (int i = 0; i < EUnitIndx.size(); ++i)
		{
			EUnitIndx[i]->GetStat().Cooltime = EUnitIndx[i]->GetStat().CooltimeOrgin * 0.9;
		}
		++m_CoolCount;
		LOG(LOG_LEVEL::LOG, L"적들의 쿨타임이 감소되었습니다.1")
	}
	else if (m_Duration > 120.f && m_CoolCount == 1)
	{
		for (int i = 0; i < EUnitIndx.size(); ++i)
		{
			EUnitIndx[i]->GetStat().Cooltime = EUnitIndx[i]->GetStat().CooltimeOrgin * 0.8;
		}
		++m_CoolCount;
		LOG(LOG_LEVEL::LOG, L"적들의 쿨타임이 감소되었습니다.2")
	}
	else if (m_Duration > 180.f && m_CoolCount == 2)
	{
		for (int i = 0; i < EUnitIndx.size(); ++i)
		{
			EUnitIndx[i]->GetStat().Cooltime = EUnitIndx[i]->GetStat().CooltimeOrgin * 0.7;
		}
		++m_CoolCount;
		LOG(LOG_LEVEL::LOG, L"적들의 쿨타임이 감소되었습니다.3")
	}
}

void EnemySommonState::SummonUnit(CUnit* _Unit)
{

	// 기지 정보 확인
	CBunker*  OwnerBunker = GetOwner<CBunker>();
	Vec2 BunkerPos = OwnerBunker->GetBodyPos();
	Vec2 BunkerScale = OwnerBunker->GetBodyScale();

	// 현재 레벨 확인
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CLevel_Stage* CurStage = dynamic_cast<CLevel_Stage*>(CurLevel);
	if (nullptr == CurStage)
	{
		LOG(LOG_LEVEL::BUG,L"제대로된 스테이지에 접근하지 못하여 소환이 불가능 합니다.")
		return;
	}

	//랜덤함수
	random_device rd;           // 하드웨어 기반의 랜덤 시드 생성
	mt19937 gen(rd());          // 메르센 트위스트 난수 생성을 rd시드로 초기화
	uniform_int_distribution<> dis(0, 100); // 0부터 100까지 균등 분포된 정수 생성

	int random_number = dis(gen);    // gen을 사용하여 0부터 100까지의 랜덤 정수 생성

	// 유닛 소환

	// 소환 맨 위 위치
	float TopPosY = BunkerPos.y - BunkerScale.y / 2.f + (_Unit->GetScale().y / 2.f + _Unit->GetBodyOffset().y);

	// 소환 맨 아리 위치
	float DownPosY = BunkerPos.y + BunkerScale.y / 2.f - (_Unit->GetScale().y / 2.f + _Unit->GetBodyOffset().y);

	// 소환 거리 설정, 결정
	float LengthY = DownPosY - TopPosY;
	Vec2 SummonPos = Vec2(BunkerPos.x + 70.f, TopPosY + (LengthY / 100.f * random_number));

	// 혹시 소환되는 유닛이 대전차병이라면 해당 레벨의 기계유닛우선타격하도록 확인, 아니면 기존소환 그대로
	if (_Unit->GetEUnitName() == ENEMY_UNIT_NAME::DESTROYER)
	{
		vector<CObj*>CurObj = CurStage->GetLayer(LAYER_TYPE::PLAYER_UNIT);
		for (int i = 0; i < CurObj.size(); ++i)
		{
			// 우선타격대상은 1. 기계, 2. 먼저 나온 유닛
			CUnit* CurUnit = dynamic_cast<CUnit*>(CurObj[i]);
			if (CurUnit->GetStat().Type == UNIT_TYPE::MACHINE)
			{
				SummonPos = Vec2(BunkerPos.x + 70.f, CurUnit->GetPos().y);
				break;
			}
		}
	}

	// 소환 위치 결정된 곳으로 소환
	CUnit* SummonUnit =_Unit->Clone();
	SummonUnit->SetPos(SummonPos);
	CurStage->AddObject(SummonUnit,LAYER_TYPE::ENEMY_UNIT);


	// 소환이 끝났으니 쿨타임 초기화
	random_device rd2;           // 하드웨어 기반의 랜덤 시드 생성
	mt19937 gen2(rd2());          // 메르센 트위스트 난수 생성을 rd시드로 초기화
	uniform_real_distribution<> dis2(1.0, 1.5); // 1부터 1.5까지 균등 분포된 실수 생성

	float random_number2 = dis2(gen2);    // gen을 사용하여 1부터 1.5까지의 랜덤 실수 생성

	Unit_Stat& CurUnitStat = _Unit->GetStat();

	CurUnitStat.CooltimeDuration = (random_number2 * CurUnitStat.Cooltime);
	wstring str2 = std::to_wstring(CurUnitStat.CooltimeDuration);
	str2 += L"설정된 쿨타임";
	LOG(LOG_LEVEL::LOG, str2.c_str())
}