#include "pch.h"
#include "EHumvee.h"


EHumvee::EHumvee()
{
	SetName(L"험비");
	// 유닛 스텟 설정
	GetStat().Armor = 10;
	GetStat().AttackDuration = 0.2f;
	GetStat().AttackSpeed = 0.2f;
	GetStat().Cooltime = 20.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 10.f;
	GetStat().FUND = 300;
	GetStat().MaxHP = 320;
	GetStat().HP = 320;
	GetStat().HDemage = 30;
	GetStat().MDemage = 30;
	GetStat().Range = Vec2(250.f, 100.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(-200.f, 0.f);


	m_EUnitName = ENEMY_UNIT_NAME::HUMVEE;
	m_BodyOffset = Vec2(0.f, 15.f);

	SetScale(Vec2(100.f, 50.f));
	SetShootType(UNIT_ATTACK::SHOOT);

	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_BulletStart = Vec2(0.f, 10.f);
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset(-((GetStat().Range.x / 2.f) + m_BulletStart.x), m_BulletStart.y);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_BASE);
	m_BulletOffset = Vec2(-40.f, 0.f);


	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"EHumvee_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"EHumvee", L"texture\\enemy\\enemy_humvee.png");
	desc.SliceSize = Vec2(192.f, 114.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EHumvee_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(384.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EHumvee_Stend";
	desc.FPS = 5;
	desc.FrmCount = 1;
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"EHumvee_Move", true);

	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"EIconUnit1", L"texture\\enemy\\UnitIcon10.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconLPos = Vec2(0.f, 80.f);
}

EHumvee::EHumvee(const EHumvee& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"EHumvee_Move", true);
}

EHumvee::~EHumvee()
{
}

void EHumvee::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"EHumvee_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void EHumvee::ShootBullet(Vec2 _Dir)
{
	m_BulletStart = GetPos() + m_BulletOffset;
	if (GetStat().AttackSpeed < GetStat().AttackDuration)
	{
		CBullet* bullet = new CSmallBullet;
		bullet->SetPos(m_BulletStart);
		bullet->SetDir(_Dir);
		bullet->SetDemage(GetStat().HDemage, GetStat().MDemage);

		CreateObject(bullet, LAYER_TYPE::ENEMY_UNIT_PROJECTILE);
		GetStat().AttackDuration = 0.f;

		PlayShootSound();

		// 이펙트 추가
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CShootBullet* neweffect = new CShootBullet;
		neweffect->SetOwner(this);
		neweffect->SetOffset(m_BulletOffset);
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
	}
}

void EHumvee::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void EHumvee::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void EHumvee::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"Bullect", L"sound\\shoot_bullet.wav");
	pBullet->Play();
}