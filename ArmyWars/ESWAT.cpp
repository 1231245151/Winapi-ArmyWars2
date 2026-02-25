#include "pch.h"
#include "ESWAT.h"


ESWAT::ESWAT()
{
	// 유닛 스텟 설정
	GetStat().Armor = 2;
	GetStat().AttackDuration = 1.2f;
	GetStat().AttackSpeed = 1.2f;
	GetStat().Cooltime = 12.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0.f;
	GetStat().FUND = 80;
	GetStat().MaxHP = 220;
	GetStat().HP = 220;
	GetStat().HDemage = 110;
	GetStat().MDemage = 140;
	GetStat().Range = Vec2(80.f, 80.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(-110.f, 0.f);


	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::INSTANCE);

	m_EUnitName = ENEMY_UNIT_NAME::SWAT;

	// 바디 설정
	m_Body->SetScale(GetScale());
	m_Body->SetOffset(m_BodyOffset);

	// 범위 설정
	m_Range->SetScale(GetStat().Range);
	m_Range->SetOffset( - (GetStat().Range.x / 2.f), 10.f);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_UNIT);
	m_Range->SetSearchLayer(LAYER_TYPE::PLAYER_BASE);
	m_BulletOffset = Vec2(-10.f, 10.f);

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"ESWAT_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"ESWAT", L"texture\\enemy\\enemy_swat.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"ESWAT_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"ESWAT_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"ESWAT_Move", true);

	// 아이콘 설정
	m_IconTexture1 = CAssetMgr::Get()->LoadTexture(L"EIconUnit1", L"texture\\enemy\\UnitIcon10.png");
	m_IconTexture2 = CAssetMgr::Get()->LoadTexture(L"pIconUnit2", L"texture\\player\\UnitIcon01.png");
	m_IconLPos = Vec2(80.f, 0.f);
}

ESWAT::ESWAT(const ESWAT& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"ESWAT_Move", true);
}

ESWAT::~ESWAT()
{
}


void ESWAT::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"ESWAT_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void ESWAT::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootShotgun* neweffect = new CShootShotgun;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset - Vec2(15.f, -5.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);

	CHitBullet* neweffect2 = new CHitBullet;
	neweffect2->SetOwner(_Other);
	neweffect2->SetOffset(Vec2(0.f, -10.f));
	CurLevel->AddObject(neweffect2, LAYER_TYPE::EFFECT);
}

void ESWAT::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"WomenVoice", L"sound\\death_women.wav");
	pvoice->Play();
}

void ESWAT::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"ShootgunBullect", L"sound\\shoot_shotgun.wav");
	pBullet->Play();
}
