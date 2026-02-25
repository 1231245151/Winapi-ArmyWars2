#include "pch.h"
#include "EBradly.h"


EBradly::EBradly()
{
	SetName(L"브래들리 장갑차");
	// 유닛 스텟 설정
	GetStat().Armor = 15;
	GetStat().AttackDuration = 0.2f;
	GetStat().AttackSpeed = 0.2f;
	GetStat().Cooltime = 35.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 60.f;
	GetStat().FUND = 700;
	GetStat().MaxHP = 600;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 40;
	GetStat().MDemage = 44;
	GetStat().Range = Vec2(300.f, 200.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(-50.f, 0.f);


	m_EUnitName = ENEMY_UNIT_NAME::BRADLY;

	SetScale(Vec2(110.f, 70.f));
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
	m_BulletOffset = Vec2(-50.f, 0.f);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"EIFV_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"EBradly", L"texture\\enemy\\enemy_striker.png");
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
	m_IconLPos = Vec2(160.f, 160.f);
}

EBradly::EBradly(const EBradly& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"EIFV_Move", true);
}

EBradly::~EBradly()
{
}

void EBradly::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"EIFV_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void EBradly::ShootBullet(Vec2 _Dir)
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

void EBradly::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void EBradly::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void EBradly::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"Bullect", L"sound\\shoot_bullet.wav");
	pBullet->Play();
}
