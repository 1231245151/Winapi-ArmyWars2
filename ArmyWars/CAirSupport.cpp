#include "pch.h"
#include "CAirSupport.h"

#include "CKeyMgr.h"
#include "CSearchRange.h"
#include "CAirBullet.h"
#include "CLogMgr.h"
#include "CLevel.h"
#include "CBunker.h"

CAirSupport::CAirSupport()
	:m_IsLeftGun(false)
	, m_Velocity(Vec2(500.f, 0.f))
{
	// 스킬 지속 시간
	m_Duration = 10.f;

	m_SkillStat.CooltimeOrgin = 40.f;
	m_SkillStat.Cooltime = m_SkillStat.CooltimeOrgin;
	m_SkillStat.CooltimeDuration = 0.f;
	m_SkillStat.FUND = 1500;
	m_SkillStat.HDemage = 150;
	m_SkillStat.MDemage = 150;
	m_SkillStat.AttackSpeed = 0.03f;
	m_SkillStat.AttackDuration = 0.f;

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"AirSupport_Move";
	desc.FPS = 10;
	desc.FrmCount = 1;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"AirSupportAnim", L"texture\\effect\\skill5_0.png");
	desc.SliceSize = Vec2(242.f, 238.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	m_Animator->Play(L"AirSupport_Move", false);

	// 사거리 설정
	m_Range = AddComponent(new CSearchRange(&m_TargetObj));
	m_Range->SetName(L"Search Range");
	m_Range->SetOffset(300, 0.f);
	m_Range->SetScale(Vec2(1.f,80.f));
	m_Range->SetSearchLayer(LAYER_TYPE::PLATFORM);

	// 아이콘 위치 설정
	m_IconLPos = Vec2(400.f, 0.f);

	// 사거리 확인 텍스쳐
	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// 스킬 UI등록용 애니메이션
	m_DomiAnim = AddComponent(new CAnimator);
	desc.AnimName = L"AirSupport_UI";
	desc.FPS = 10;
	desc.FrmCount = 1;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"SkillIcon1", L"texture\\stage\\SkillIcon00.png");
	desc.SliceSize = Vec2(80.f, 80.f);
	desc.StartLeftTop = Vec2(400.f, 0.f);
	m_DomiAnim->CreateAnimation(desc);

	m_DomiAnim->Play(L"AirSupport_UI", false);

}

CAirSupport::CAirSupport(const CAirSupport& _Other)
	: CSkill(_Other)
	, m_Velocity(_Other.m_Velocity)
	, m_Range(nullptr)
	, m_TargetObj(_Other.m_TargetObj)
	, m_IsLeftGun(false)
{
	m_Range = GetComponent<CSearchRange>();
	m_Range->SetVector(&m_TargetObj);
}

CAirSupport::~CAirSupport()
{
}

void CAirSupport::Tick()
{
	CSkill::Tick();

	// 공격 속도 처리 계산
	if (m_SkillStat.AttackDuration < m_SkillStat.AttackSpeed)
		m_SkillStat.AttackDuration += DT;
	else
		m_SkillStat.AttackDuration = m_SkillStat.AttackSpeed;

	// 공격 타이밍이라면 공격 개시
	if (0 < m_TargetObj.size() && m_SkillStat.AttackDuration >= m_SkillStat.AttackSpeed)
	{
		Vec2 BulletPos;
		Vec2 vDir;
		Vec2 vPos;
		if (m_IsLeftGun)
		{
			// 총알 목적지
			BulletPos = Vec2(m_Range->GetPos().x, m_Range->GetPos().y - 30);

			// 총알 발사 지점
			vPos = Vec2(GetPos().x + 50, GetPos().y - 60);

			//방향지정
			vDir = BulletPos - vPos;
			vDir.Normalize();

			m_IsLeftGun = false;
		}
		else if(!m_IsLeftGun)
		{
			// 총알 목적지
			BulletPos = Vec2(m_Range->GetPos().x, m_Range->GetPos().y + 10);

			// 총알 발사 지점
			vPos = Vec2(GetPos().x + 50, GetPos().y - 30);

			//방향지정
			vDir = BulletPos - vPos;
			vDir.Normalize();

			m_IsLeftGun = true;

			// 너무 시끄러워서 여기에만 사운드 들림
			CSound* pBullet = CAssetMgr::Get()->LoadSound(L"AirBullect", L"sound\\shoot_airbullet.wav");
			pBullet->Play();
		}

		// 총알 생성
		CAirBullet* newBullet = new CAirBullet;
		newBullet->SetPos(vPos);
		newBullet->SetDir(vDir);
		newBullet->SetDemage(GetStat().HDemage, GetStat().MDemage);
		newBullet->SetTargetPos(BulletPos);

		CreateObject(newBullet, LAYER_TYPE::PLAYER_UNIT_PROJECTILE);


		// 공격주기 초기화
		m_SkillStat.AttackDuration = 0.f;
	}

	// 이동처리
	Vec2 vPos = GetPos();
	vPos += m_Velocity * DT;
	SetPos(vPos);
}

void CAirSupport::Render()
{
	CSkill::Render();

	// 중심점 확인용
	//DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::RED, BRUSH_TYPE::HOLLOW, GetRenderPos(), Vec2(10.f,10.f));
}

void CAirSupport::ActiveSkill()
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

	float UnderY = vDiff.y + m_Range->GetScale().y / 2.f -20.f;
	float UpY = vDiff.y - m_Range->GetScale().y / 2.f + 20.f;

	//전장 위치를 벗어난곳이 아님을 확인하면 소환
	if (fieldScale.y / 2.f >= UnderY && fieldScale.y / 2.f >= UpY)
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

		// 자기자신을 복사 생성하여 위치 맞춰주고 현재 레벨에 삽입
		CSkill* newSkill = this->Clone();
		newSkill->SetPos(Target->GetPos().x - 600.f, MouseRealPos.y);
		CurLevel->AddObject(newSkill, LAYER_TYPE::PLAYER_SKILL);

		// 스킬 쿨타임 돌려지도록 CooltimeDuration을 CoolTime에 맞추기
		m_SkillStat.CooltimeDuration = m_SkillStat.Cooltime;

		// 출격 사운드 출력
		CSound* pBullet = CAssetMgr::Get()->LoadSound(L"AirSkill", L"sound\\skill_airstrike.wav");
		pBullet->Play();

		// 보이스
		CSound* pvoice = CAssetMgr::Get()->LoadSound(L"Airvoice", L"sound\\skill_airvoice.wav");
		pvoice->Play();
	}
	// 올바른 위치가 아니라면 강제 종료
	else
	{
		return;
	}
}