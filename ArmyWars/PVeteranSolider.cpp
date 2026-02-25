#include "pch.h"
#include "PVeteranSolider.h"



PVeteranSolider::PVeteranSolider()
{
	SetName(L"º£Å×¶û ¼ÒÃÑº´");
	// À¯´Ö ½ºÅÝ ¼³Á¤
	GetStat().Armor = 13;
	GetStat().AttackDuration = 0.25f;
	GetStat().AttackSpeed = 0.25f;
	GetStat().Cooltime = 20.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 300;
	GetStat().MaxHP = 420;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 42;
	GetStat().MDemage = 33;
	GetStat().Range = Vec2(300.f, 180.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(100.f, 0.f);

	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::SHOOT);

	m_PUnitName = PLAYER_UNIT_NAME::VETERANSOLIDER;

	// ¹Ùµð ¼³Á¤
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// ¹üÀ§ ¼³Á¤
	m_BulletStart = Vec2(0.f, 10.f);
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset((GetStat().Range.x / 2.f) + m_BulletStart.x, m_BulletStart.y);
	m_Range->SetSearchLayer(LAYER_TYPE::ENEMY_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::ENEMY_BASE);

	//wstring str = L"RangeUI" + std::to_wstring((int)PLAYER_UNIT_NAME::SOLIDER);
	//m_RangeTexture = CAssetMgr::Get()->CreateTexture(str, GetStat().Range.x, GetStat().Range.y);
	//SELECT_BRUSH(m_RangeTexture->GetDC(), BRUSH_TYPE::RED);
	//Rectangle(m_RangeTexture->GetDC(), 0, 0, GetStat().Range.x, GetStat().Range.y);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation ¼³Á¤
	tAnimDesc desc = {};
	desc.AnimName = L"PveteranSolider_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"PveteranSolider", L"texture\\player\\player_veteransolider.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PveteranSolider_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PveteranSolider_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PveteranSolider_Move", true);

	m_IconLPos = Vec2(320.f, 80.f);
}

PVeteranSolider::PVeteranSolider(const PVeteranSolider& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PveteranSolider_Move", true);
}

PVeteranSolider::~PVeteranSolider()
{
}

void PVeteranSolider::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PveteranSolider_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PVeteranSolider::ShootBullet(Vec2 _Dir)
{
	m_BulletStart = GetPos() + m_BulletOffset;
	if (GetStat().AttackSpeed < GetStat().AttackDuration)
	{
		CBullet* bullet = new CSmallBullet;
		bullet->SetPos(m_BulletStart);
		bullet->SetDir(_Dir);
		bullet->SetDemage(GetStat().HDemage, GetStat().MDemage);

		CreateObject(bullet, LAYER_TYPE::PLAYER_UNIT_PROJECTILE);
		GetStat().AttackDuration = 0.f;

		PlayShootSound();

		// ÀÌÆåÆ® Ãß°¡
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CShootBullet* neweffect = new CShootBullet;
		neweffect->SetOwner(this);
		neweffect->SetOffset(m_BulletOffset);
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
	}
}

void PVeteranSolider::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// ÀÌÆåÆ® Ãß°¡
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void PVeteranSolider::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MenVoice", L"sound\\death_men.wav");
	pvoice->Play();
}

void PVeteranSolider::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"SmallBullect", L"sound\\shoot_smallbullet.wav");
	pBullet->Play();
}