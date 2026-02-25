#include "pch.h"
#include "PSWAT.h"


PSWAT::PSWAT()
{
	SetName(L"기동대원");
	// 유닛 스텟 설정
	GetStat().Armor = 10;
	GetStat().AttackDuration = 1.2f;
	GetStat().AttackSpeed = 1.2f;
	GetStat().Cooltime = 13.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 80;
	GetStat().MaxHP = 220;
	GetStat().HP = 220;
	GetStat().HDemage = 120;
	GetStat().MDemage = 150;
	GetStat().Range = Vec2(80.f, 80.f);
	GetStat().Type = UNIT_TYPE::HUMEN;
	GetStat().velocity = Vec2(110.f, 0.f);

	m_PUnitName = PLAYER_UNIT_NAME::SWAT;


	SetScale(Vec2(30.f, 30.f));
	SetShootType(UNIT_ATTACK::INSTANCE);

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
	desc.AnimName = L"PSWAT_Move";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"pPSWAT", L"texture\\player\\player_swat.png");
	desc.SliceSize = Vec2(93.f, 63.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PSWAT_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(372.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PSWAT_Stend";
	desc.FPS = 5;
	desc.FrmCount = 3;
	desc.StartLeftTop = Vec2(744.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PSWAT_Move", true);

	m_IconLPos = Vec2(320.f, 0.f);
}

PSWAT::PSWAT(const PSWAT& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PSWAT_Move", true);
}

PSWAT::~PSWAT()
{
}


void PSWAT::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PSWAT_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PSWAT::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootShotgun* neweffect = new CShootShotgun;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset + Vec2(10.f,5.f));
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);

	

	CHitBullet* neweffect2 = new CHitBullet;
	neweffect2->SetOwner(_Other);
	neweffect2->SetOffset(Vec2(0.f, -10.f));
	CurLevel->AddObject(neweffect2, LAYER_TYPE::EFFECT);
}

void PSWAT::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"WomenVoice", L"sound\\death_women.wav");
	pvoice->Play();
}

void PSWAT::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"ShootgunBullect", L"sound\\shoot_shotgun.wav");
	pBullet->Play();
}
