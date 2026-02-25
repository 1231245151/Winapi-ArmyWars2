#include "pch.h"
#include "CRepair.h"

#include "CLevelMgr.h"
#include "CLogMgr.h"
#include "CLevel.h"
#include "CBunker.h"

CRepair::CRepair()
{
	// 스킬 지속 시간
	m_Duration = 2.f;

	m_SkillStat.CooltimeOrgin = 60.f;
	m_SkillStat.Cooltime = m_SkillStat.CooltimeOrgin;
	m_SkillStat.CooltimeDuration = 0.f;
	m_SkillStat.FUND = 500;
	m_SkillStat.HDemage = 500;
	
	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"Repair";
	desc.FPS = 10;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"RepairAnim", L"texture\\effect\\effect_repair.png");
	desc.SliceSize = Vec2(30.f, 30.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	m_Animator->Play(L"Repair", true);

	// 아이콘 위치 설정
	m_IconLPos = Vec2(320.f, 0.f);
}

CRepair::~CRepair()
{
}

void CRepair::Tick()
{
	CSkill::Tick();
}

void CRepair::Render()
{
	CSkill::Render();
}

void CRepair::ActiveSkill()
{
	//현재 레벨에 접근
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// 해당 레벨에 아군 Base 레이어 찾기(1개만 존재할 테니 0번쨰 레이어를 가져온다
	vector<CObj*>& pBase = CurLevel->GetLayer(LAYER_TYPE::PLAYER_BASE);
	CBunker* Target = dynamic_cast<CBunker*>(pBase[0]);

	if (nullptr == Target)
	{
		LOG(LOG_LEVEL::BUG, L"플레이어의 벙커를 찾지 못하여 스킬 사용 강제 종료")
		return;
	}

	// 해당 베이스의 스텟 가져오기
	Unit_Stat& BaseStat = Target->GetBaseStat();

	// 베이스 코스트를 스킬 코스트만큼 감소 시키고 체력 회복시키기 체력이 Max를 넘으면 Max수치까지만
	// 코스트가 맞지 않는데 호출되었다면 그냥 return
	if (Target->GetHaveFUND() < m_SkillStat.FUND)
		return;

	Target->GetHaveFUND() -= m_SkillStat.FUND;
	BaseStat.HP += m_SkillStat.HDemage;
	if (BaseStat.HP > BaseStat.MaxHP)
		BaseStat.HP = BaseStat.MaxHP;

	// 자기자신을 복사 생성하여 위치 맞춰주고 현재 레벨에 삽입
	CSkill* newSkill = this->Clone();
	newSkill->SetPos(Target->GetPos());
	CurLevel->AddObject(newSkill, LAYER_TYPE::PLAYER_SKILL);

	// 스킬 쿨타임 돌려지도록 CooltimeDuration을 CoolTime에 맞추기
	m_SkillStat.CooltimeDuration = m_SkillStat.Cooltime;

	// 보이스
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"Repairvoice", L"sound\\skill_repairvoice.wav");
	pvoice->Play();
}
