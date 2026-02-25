#include "pch.h"
#include "PSniper.h"

PSniper::PSniper()
{
	SetName(L"저격병");
	// 유닛 스텟 설정
	GetStat().Armor = 3;
	GetStat().AttackDuration = 7.f;
	GetStat().AttackSpeed = 7.f;
	GetStat().Cooltime = 20;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 80;
	GetStat().MaxHP = 100;
	GetStat().HP = 100;
	GetStat().HDemage = 250;
	GetStat().MDemage = 30;
	GetStat().Range = Vec2(600.f, 30.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(40.f, 0.f);

	m_PUnitName = PLAYER_UNIT_NAME::SNIPER;


	SetScale(Vec2(30.f, 30.f));
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

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"PSniper_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"pSniper", L"texture\\player\\player_sniper.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PSniper_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PSniper_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PSniper_Move", true);

	m_IconLPos = Vec2(240.f, 0.f);
}

PSniper::PSniper(const PSniper& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PSniper_Move", true);
}

PSniper::~PSniper()
{
}

void PSniper::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PSniper_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PSniper::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void PSniper::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MenVoice", L"sound\\death_men.wav");
	pvoice->Play();
}

void PSniper::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"SniperBullect", L"sound\\shoot_sniper.wav");
	pBullet->Play();
}
