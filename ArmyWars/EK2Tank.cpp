#include "pch.h"
#include "EK2Tank.h"
#include "CShootTank2.h"

EK2Tank::EK2Tank()
{
	SetName(L"K2 탱크");
	// 유닛 스텟 설정
	GetStat().Armor = 15;
	GetStat().AttackDuration = 3.f;
	GetStat().AttackSpeed = 3.f;
	GetStat().Cooltime = 80.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 110.f;
	GetStat().FUND = 3200;
	GetStat().MaxHP = 2500;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 230;
	GetStat().MDemage = 400;
	GetStat().Range = Vec2(280.f, 200.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(-50.f, 0.f);


	m_EUnitName = ENEMY_UNIT_NAME::K2TANK;

	SetScale(Vec2(100.f, 70.f));
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
	m_BulletOffset = Vec2(-100.f, -10.f);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"EIFV_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"EK2Tank", L"texture\\enemy\\enemy_k2tank.png");
	desc.SliceSize = Vec2(192.f, 114.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EIFV_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(384.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EIFV_Stend";
	desc.FPS = 5;
	desc.FrmCount = 1;
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"EIFV_Move", true);

	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"EIconUnit1", L"texture\\enemy\\UnitIcon10.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconLPos = Vec2(0.f, 240.f);
}

EK2Tank::EK2Tank(const EK2Tank& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"EIFV_Move", true);
}

EK2Tank::~EK2Tank()
{
}

void EK2Tank::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"EIFV_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void EK2Tank::ShootBullet(Vec2 _Dir)
{
	m_BulletStart = GetPos() + m_BulletOffset;
	if (GetStat().AttackSpeed < GetStat().AttackDuration)
	{
		CBullet* bullet = new CTankBullet;
		bullet->SetPos(m_BulletStart);
		bullet->SetDir(_Dir);
		bullet->SetDemage(GetStat().HDemage, GetStat().MDemage);

		CreateObject(bullet, LAYER_TYPE::ENEMY_UNIT_PROJECTILE);
		GetStat().AttackDuration = 0.f;

		PlayShootSound();

		// 이펙트 추가
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CShootTank2* neweffect = new CShootTank2;
		neweffect->SetOwner(this);
		neweffect->SetOffset(m_BulletOffset + Vec2(-10.f, 0.f));
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
	}
}

void EK2Tank::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootTank2* neweffect = new CShootTank2;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void EK2Tank::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void EK2Tank::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"bazukaBullect", L"sound\\shoot_bazuka.wav");
	pBullet->Play();
}