#include "pch.h"
#include "EHeavySolider.h"



EHeavySolider::EHeavySolider()
{
	SetName(L"중화기병");
	// 유닛 스텟 설정
	GetStat().Armor = 8;
	GetStat().AttackDuration = 0.19f;
	GetStat().AttackSpeed = 0.19f;
	GetStat().Cooltime = 20.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 1.f;
	GetStat().FUND = 120;
	GetStat().MaxHP = 320;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 20;
	GetStat().MDemage = 28;
	GetStat().Range = Vec2(150.f, 300.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(-40.f, 0.f);

	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::SHOOT);

	m_EUnitName = ENEMY_UNIT_NAME::HEAVYSOLIDR;

	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_BulletStart = Vec2(0.f, 10.f);
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset(-((GetStat().Range.x / 2.f) + m_BulletStart.x), m_BulletStart.y);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_BASE);
	m_BulletOffset = Vec2(-10.f, 10.f);

	//wstring str = L"RangeUI" + std::to_wstring((int)PLAYER_UNIT_NAME::SOLIDER);
	//m_RangeTexture = CAssetMgr::Get()->CreateTexture(str, GetStat().Range.x, GetStat().Range.y);
	//SELECT_BRUSH(m_RangeTexture->GetDC(), BRUSH_TYPE::RED);
	//Rectangle(m_RangeTexture->GetDC(), 0, 0, GetStat().Range.x, GetStat().Range.y);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"EHeavySolider_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"EHeavySolider", L"texture\\enemy\\enemy_heavysolider.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EHeavySolider_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"EHeavySolider_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"EHeavySolider_Move", true);

	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"EIconUnit1", L"texture\\enemy\\UnitIcon10.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconLPos = Vec2(320.f, 0.f);
}

EHeavySolider::EHeavySolider(const EHeavySolider& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"EHeavySolider_Move", true);
}

EHeavySolider::~EHeavySolider()
{
}

void EHeavySolider::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"EHeavySolider_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void EHeavySolider::ShootBullet(Vec2 _Dir)
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

void EHeavySolider::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void EHeavySolider::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"men2Voice", L"sound\\death_men2.wav");
	pvoice->Play();
}

void EHeavySolider::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"SmallBullect", L"sound\\shoot_smallbullet.wav");
	pBullet->Play();
}