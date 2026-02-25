#include "pch.h"
#include "PMedic.h"

#include "CHitHeal.h"

PMedic::PMedic()
{
	SetName(L"의무병");
	// 유닛 스텟 설정
	GetStat().Armor = 5;
	GetStat().AttackDuration = 2.5f;
	GetStat().AttackSpeed = 2.5f;
	GetStat().Cooltime = 15.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0.f;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 70;
	GetStat().MaxHP = 120;
	GetStat().HP = 120;
	GetStat().HDemage = 130;
	GetStat().MDemage = 0;
	GetStat().Range = Vec2(200.f, 150.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(60.f, 0.f);

	m_PUnitName = PLAYER_UNIT_NAME::MEDIC;

	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::HEAL);


	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_BulletStart = Vec2(0.f, 10.f);
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset((GetStat().Range.x / 2.f) + m_BulletStart.x, m_BulletStart.y);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeBlue", L"texture\\effect\\Range_Blue.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"PMedic_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"pPMedic", L"texture\\player\\player_medic.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PMedic_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PMedic_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PMedic_Move", true);

	m_IconLPos = Vec2(0.f, 0.f);
}

PMedic::PMedic(const PMedic& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PMedic_Move", true);
}

PMedic::~PMedic()
{
}

void PMedic::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PMedic_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PMedic::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	Unit_Stat& stat = GetStat();
	// 공격을 한 대상 타입이 인간
	if (UNIT_TYPE::HUMEN == _EnemyStat.Type)
	{
		// 치유량이 최대체력보다 넘는다면 최대체력으로 고정
		if (stat.HDemage > _EnemyStat.MaxHP - _EnemyStat.HP)
			_EnemyStat.HP = _EnemyStat.MaxHP;
		else
			_EnemyStat.HP += stat.HDemage;
	}
	// 공격을 한 대상 타입이 기계
	else if (UNIT_TYPE::MACHINE == _EnemyStat.Type)
	{
		// 치유량이 최대체력보다 넘는다면 최대체력으로 고정
		if (stat.MDemage > _EnemyStat.MaxHP - _EnemyStat.HP)
			_EnemyStat.HP = _EnemyStat.MaxHP;
		else
			_EnemyStat.HP += stat.MDemage;
	}
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CHitHeal* neweffect = new CHitHeal;
	neweffect->SetOwner(_Other);
	neweffect->SetOffset(Vec2(0.f, -10.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void PMedic::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"WomenVoice", L"sound\\death_women.wav");
	pvoice->Play();
}

void PMedic::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"HealBullect", L"sound\\shoot_heal.wav");
	pBullet->Play();
}
