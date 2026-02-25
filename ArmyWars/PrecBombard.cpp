#include "pch.h"
#include "PrecBombard.h"

#include "CKeyMgr.h"
#include "CSearchRange.h"
#include "CLogMgr.h"
#include "CLevel.h"
#include "CBunker.h"
#include "CPrecBoom.h"

PrecBombard::PrecBombard()
{
	// 스킬 지속 시간
	m_Duration = 10.f;

	m_SkillStat.CooltimeOrgin = 30.f;
	m_SkillStat.Cooltime = m_SkillStat.CooltimeOrgin;
	m_SkillStat.CooltimeDuration = 0.f;
	m_SkillStat.FUND = 2000;
	m_SkillStat.HDemage = 2200;
	m_SkillStat.MDemage = 2200;
	m_SkillStat.AttackSpeed = 0.03f;
	m_SkillStat.AttackDuration = 0.f;

	SetScale(Vec2(140.f, 140.f));


	// 사거리 설정
	m_Range = AddComponent(new CSearchRange(&m_TargetObj));
	m_Range->SetName(L"Search Range");
	m_Range->SetOffset(0.f, 0.f);
	m_Range->SetScale(Vec2(140.f,140.f));
	m_Range->SetSearchLayer(LAYER_TYPE::PLATFORM);

	// 아이콘 위치 설정
	m_IconLPos = Vec2(240.f, 80.f);

	// 사거리 확인 텍스쳐
	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// 스킬 UI등록용 애니메이션
	m_DomiAnim = AddComponent(new CAnimator);
	tAnimDesc desc = {};
	desc.AnimName = L"AirSupport_UI";
	desc.FPS = 10;
	desc.FrmCount = 1;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png");
	desc.SliceSize = Vec2(80.f, 80.f);
	desc.StartLeftTop = Vec2(240.f, 80.f);
	m_DomiAnim->CreateAnimation(desc);

	m_DomiAnim->Play(L"AirSupport_UI", false);
}

PrecBombard::PrecBombard(const PrecBombard& _Other)
	: CSkill(_Other)
	, m_Velocity(_Other.m_Velocity)
	, m_Range(nullptr)
{
	m_Range = GetComponent<CSearchRange>();
}

PrecBombard::~PrecBombard()
{
}

void PrecBombard::Tick()
{
}

void PrecBombard::Render()
{
}

void PrecBombard::ActiveSkill()
{
	//현재 레벨에 접근
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// 해당 스킬이 올바른 위치에 놓았는지 확인
	Vec2 MouseRealPos = CCamera::Get()->GetRealPos(CKeyMgr::Get()->GetMousePos());
	vector<CObj*>& Platform = CurLevel->GetLayer(LAYER_TYPE::PLATFORM);
	Vec2 fieldScale = Platform[0]->GetScale();
	Vec2 fieldPos = Platform[0]->GetPos();

	Vec2 vDiff = fieldPos - MouseRealPos;

	vDiff.y = abs(vDiff.y);
	vDiff.x = abs(vDiff.x);

	float UnderY = vDiff.y + m_Range->GetScale().y / 2.f - 50.f;
	float UpY = vDiff.y - m_Range->GetScale().y / 2.f + 50.f;

	float LeftX = vDiff.x + m_Range->GetScale().y / 2.f - 20.f;
	float RightX = vDiff.y - m_Range->GetScale().y / 2.f + 20.f;

	//전장 위치를 벗어난곳이 아님을 확인하면 소환
	if (fieldScale.y / 2.f >= UnderY && fieldScale.y / 2.f >= UpY && fieldScale.x / 2.f >= LeftX && fieldScale.x / 2.f >= RightX)
	{
		// 해당 레벨에 아군 Base 레이어 찾기(1개만 존재할 테니 0번쨰 레이어를 가져온다
		vector<CObj*>& pBase = CurLevel->GetLayer(LAYER_TYPE::PLAYER_BASE);
		CBunker* Target = dynamic_cast<CBunker*>(pBase[0]);

		// 벙커를 못찾앗다면 강제 종료
		if (nullptr == Target)
		{
			LOG(LOG_LEVEL::BUG, L"플레이어의 벙커를 찾지 못하여 스킬 사용 강제 종료")
				return;
		}

		// 해당 베이스의 스텟 가져오기
		Unit_Stat& BaseStat = Target->GetBaseStat();

		// 코스트가 맞지 않는데 호출되었다면 그냥 return
		if (Target->GetHaveFUND() < m_SkillStat.FUND)
			return;

		// 베이스 코스트를 스킬 코스트만큼 감소 시키기
		Target->GetHaveFUND() -= m_SkillStat.FUND;



		// 폭탄 투하용 총알 생성

		// 총알 목적지
		Vec2 BulletPos = Vec2(MouseRealPos.x, MouseRealPos.y);

		// 총알 발사 지점
		Vec2 vPos = Vec2(MouseRealPos.x, MouseRealPos.y - 400);

		//방향지정
		Vec2 vDir = BulletPos - vPos;
		vDir.Normalize();


		CPrecBoom* newBullet = new CPrecBoom;
		newBullet->SetPos(vPos);
		newBullet->SetDir(vDir);
		newBullet->SetDemage(GetStat().HDemage, GetStat().MDemage);
		newBullet->SetTargetPos(BulletPos);
		CurLevel->AddObject(newBullet, LAYER_TYPE::PLAYER_UNIT_PROJECTILE);






		// 스킬 쿨타임 돌려지도록 CooltimeDuration을 CoolTime에 맞추기
		m_SkillStat.CooltimeDuration = m_SkillStat.Cooltime;


		// 보이스
		CSound* pvoice = CAssetMgr::Get()->LoadSound(L"boomvoice", L"sound\\skill_precboomvoice.wav");
		pvoice->Play();
	}
	// 올바른 위치가 아니라면 강제 종료
	else
	{
		LOG(LOG_LEVEL::LOG, L"적절한 위치가 아니라서 스킬 사용 강제 종료")
		return;
	}
}