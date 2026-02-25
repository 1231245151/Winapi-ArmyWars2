#include "pch.h"
#include "PK2Tank.h"

#include "CShootTank.h"

PK2Tank::PK2Tank()
{
	SetName(L"K2 전차");
	// 유닛 스텟 설정
	GetStat().Armor = 25;
	GetStat().AttackDuration = 2.5f;
	GetStat().AttackSpeed = 2.5f;
	GetStat().Cooltime = 70.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 3500;
	GetStat().MaxHP = 2700;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 250;
	GetStat().MDemage = 430;
	GetStat().Range = Vec2(280.f, 200.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(20.f, 0.f);


	m_PUnitName = PLAYER_UNIT_NAME::K2TANK;

	SetScale(Vec2(100.f, 70.f));
	SetShootType(UNIT_ATTACK::SHOOT);

	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_BulletStart = Vec2(0.f, 10.f);
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset((GetStat().Range.x / 2.f) + m_BulletStart.x, m_BulletStart.y);
	m_Range->SetSearchLayer(LAYER_TYPE::ENEMY_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::ENEMY_BASE);
	m_BulletOffset = Vec2(100.f, -10.f);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"PK2_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"PK2", L"texture\\player\\player_k2.png");
	desc.SliceSize = Vec2(192.f, 114.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PK2_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(384.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PK2_Stend";
	desc.FPS = 5;
	desc.FrmCount = 1;
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PK2_Move", true);

	m_IconLPos = Vec2(400.f, 240.f);
}

PK2Tank::PK2Tank(const PK2Tank& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PK2_Move", true);
}

PK2Tank::~PK2Tank()
{
}

void PK2Tank::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PK2_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PK2Tank::ShootBullet(Vec2 _Dir)
{
	m_BulletStart = GetPos() + m_BulletOffset;
	if (GetStat().AttackSpeed < GetStat().AttackDuration)
	{
		CBullet* bullet = new CTankBullet;
		bullet->SetPos(m_BulletStart);
		bullet->SetDir(_Dir);
		bullet->SetDemage(GetStat().HDemage, GetStat().MDemage);

		CreateObject(bullet, LAYER_TYPE::PLAYER_UNIT_PROJECTILE);
		GetStat().AttackDuration = 0.f;

		PlayShootSound();

		// 이펙트 추가
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CShootTank* neweffect = new CShootTank;
		neweffect->SetOwner(this);
		neweffect->SetOffset(m_BulletOffset + Vec2(10.f,0.f));
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
	}
}

void PK2Tank::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootTank* neweffect = new CShootTank;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset + Vec2(10.f, 0.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void PK2Tank::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void PK2Tank::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"bazukaBullect", L"sound\\shoot_bazuka.wav");
	pBullet->Play();
}