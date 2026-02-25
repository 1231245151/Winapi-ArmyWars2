#include "pch.h"
#include "EVeteranSolider.h"



EVeteranSolider::EVeteranSolider()
{
	SetName(L"베테랑 소총병");
	// 유닛 스텟 설정
	GetStat().Armor = 10;
	GetStat().AttackDuration = 0.25f;
	GetStat().AttackSpeed = 0.25f;
	GetStat().Cooltime = 25.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 20.f;
	GetStat().FUND = 300;
	GetStat().MaxHP = 420;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 41;
	GetStat().MDemage = 32;
	GetStat().Range = Vec2(300.f, 180.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(-100.f, 0.f);


	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::SHOOT);

	m_EUnitName = ENEMY_UNIT_NAME::VETERANSOLIDER;


	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset(-(GetStat().Range.x / 2.f), 10.f);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_BASE);
	m_BulletOffset = Vec2(-10.f, 10.f);

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"ESolider_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"eveteranSolider", L"texture\\enemy\\enemy_veteransolider.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"ESolider_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"ESolider_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"ESolider_Move", true);

	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"EIconUnit1", L"texture\\enemy\\UnitIcon10.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconLPos = Vec2(80.f, 80.f);
}

EVeteranSolider::EVeteranSolider(const EVeteranSolider& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"ESolider_Move", true);
}

EVeteranSolider::~EVeteranSolider()
{
}

void EVeteranSolider::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"ESolider_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void EVeteranSolider::ShootBullet(Vec2 _Dir)
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

void EVeteranSolider::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void EVeteranSolider::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MenVoice", L"sound\\death_men.wav");
	pvoice->Play();
}

void EVeteranSolider::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"SmallBullect", L"sound\\shoot_smallbullet.wav");
	pBullet->Play();
}