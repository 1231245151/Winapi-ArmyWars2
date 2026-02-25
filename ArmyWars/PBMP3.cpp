#include "pch.h"
#include "PBMP3.h"


PBMP3::PBMP3()
{
	SetName(L"BMP3 장갑차");
	// 유닛 스텟 설정
	GetStat().Armor = 20;
	GetStat().AttackDuration = 0.35f;
	GetStat().AttackSpeed = 0.35f;
	GetStat().Cooltime = 36.f;
	GetStat().CooltimeOrgin = GetStat().Cooltime;
	GetStat().CooltimeDuration = 0;
	GetStat().firstCooltime = 0;
	GetStat().FUND = 1000;
	GetStat().MaxHP = 800;
	GetStat().HP = GetStat().MaxHP;
	GetStat().HDemage = 40;
	GetStat().MDemage = 60;
	GetStat().Range = Vec2(300.f, 200.f);
	GetStat().Type = UNIT_TYPE::MACHINE;
	GetStat().velocity = Vec2(50.f, 0.f);


	m_PUnitName = PLAYER_UNIT_NAME::IFV;

	SetScale(Vec2(110.f, 70.f));
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
	m_BulletOffset = Vec2(50.f, 0.f);

	m_RangeTexture = CAssetMgr::Get()->LoadTexture(L"RangeRed", L"texture\\effect\\Range_Red.png");

	// Animation 설정
	tAnimDesc desc = {};
	desc.AnimName = L"PIFV_Move";
	desc.FPS = 5;
	desc.FrmCount = 2;
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"PIFV", L"texture\\player\\player_ifv.png");
	desc.SliceSize = Vec2(192.f, 114.f);
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PIFV_Death";
	desc.FPS = 5;
	desc.FrmCount = 4;
	desc.StartLeftTop = Vec2(384.f, 0.f);
	m_Animator->CreateAnimation(desc);

	desc.AnimName = L"PIFV_Stend";
	desc.FPS = 5;
	desc.FrmCount = 1;
	desc.StartLeftTop = Vec2(0.f, 0.f);
	m_Animator->CreateAnimation(desc);


	m_Animator->Play(L"PIFV_Move", true);

	m_IconLPos = Vec2(80.f, 160.f);
}

PBMP3::PBMP3(const PBMP3& _Other)
	:CUnit(_Other)
{
	m_Animator->Play(L"PIFV_Move", true);
}

PBMP3::~PBMP3()
{
}

void PBMP3::PlayAnimation(const wstring& _Name, bool _Repeat)
{
	wstring str = L"PIFV_" + _Name;
	m_Animator->Play(str, _Repeat);
}

void PBMP3::Demagecall(Unit_Stat& _EnemyStat, CObj* _Other)
{
	CUnit::Demagecall(_EnemyStat, _Other);
	// 이펙트 추가
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CShootBullet* neweffect = new CShootBullet;
	neweffect->SetOwner(this);
	neweffect->SetOffset(m_BulletOffset);
	CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);
}

void PBMP3::PlayDeathSound()
{
	CSound* pvoice = CAssetMgr::Get()->LoadSound(L"MachineDeath", L"sound\\death_machine.wav");
	pvoice->Play();
}

void PBMP3::PlayShootSound()
{
	CSound* pBullet = CAssetMgr::Get()->LoadSound(L"Bullect", L"sound\\shoot_bullet.wav");
	pBullet->Play();
}