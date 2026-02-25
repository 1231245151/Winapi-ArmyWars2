#include "pch.h"
#include "ETruck.h"



ETruck::ETruck()
{
	SetName(L"트럭");
	// 유닛 스텟 설정
	GetStat().Armor = 20;
	GetStat().AttackDuration = 0.3f;
	GetStat().AttackSpeed = 0.3f;
	GetStat().Cooltime = 33.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 10.f;
	GetStat().FUND = 400;
	GetStat().MaxHP = 630;
	GetStat().HP = 630;
	GetStat().HDemage = 30;
	GetStat().MDemage = 30;
	GetStat().Range = Vec2(250.f, 150.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(-80.f, 0.f);


	m_EUnitName = ENEMY_UNIT_NAME::TRUCK;

	SetScale(Vec2(110.f, 60.f));
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
	m_BulletOffset = Vec2(-60.f, -20.f);


	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"EHumvee_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"ETruck", L"texture\\enemy\\enemy_truck.png");
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
	m_IconLPos = Vec2(400.f, 80.f);
}

ETruck::ETruck(const ETruck& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"EHumvee_Move", true);
}

ETruck::~ETruck()
{
}

void ETruck::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"EHumvee_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void ETruck::ShootBullet(Vec2 _Dir)
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

void ETruck::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void ETruck::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void ETruck::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"Bullect", L"sound\\shoot_bullet.wav");
	pBullet->Play();
}