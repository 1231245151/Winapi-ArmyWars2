#include "pch.h"
#include "CBullet.h"

#include "CUnit.h"
#include "CTexture.h"
#include "CHitHumen.h"

CBullet::CBullet()
	:m_Body(nullptr)
	,m_Duration(30.f)
	, m_Hit(false)
	,m_speed(100.f)
	,m_Texture(nullptr)
	,m_Dir(0.f,0.f)
{
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"Bullet CCollider");
}

//CBullet::CBullet(const CBullet& _Other)
//{
//}

CBullet::~CBullet()
{
}

void CBullet::Tick()
{
	// 시간 지나면 없어짐

	m_Duration -= DT;
	
	if (0.f >= m_Duration)
		DeleteObject(this);


	//총알 이동
	Vec2 vPos = GetPos();
	vPos += (m_Dir * m_speed) * DT;
	SetPos(vPos);

}

void CBullet::Render()
{

	// 알파 랜더링
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 0(투명) ~ 255(불투명)
	blend.AlphaFormat = AC_SRC_ALPHA;

	// 랜더링 할 위치 결정
	Vec2 RenderPos = GetRenderPos();
	Vec2 RenderScale = GetScale();

	// 알파 블랜딩 랜더링
	AlphaBlend(BackDC, (int)(RenderPos.x - m_Texture->GetWidth() / 2), (int)(RenderPos.y - m_Texture->GetHeight() / 2)
		, (int)(m_Texture->GetWidth()), (int)(m_Texture->GetHeight())
		, m_Texture->GetDC()
		, (int)0, 0
		, (int)m_Texture->GetWidth(), (int)m_Texture->GetHeight()
		, blend);


}



void CBullet::Demagecall(CUnit* _type)
{
	Unit_Stat& stat = _type->GetStat();
	
	if (UNIT_TYPE::HUMEN == stat.Type)
	{
		int demage = m_HDemage - stat.Armor;
		if (demage > 0)
			stat.HP -= demage;

		// 이펙트 추가
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CHitHumen* neweffect = new CHitHumen;
		CObj* Obj = (CObj*)_type;
		neweffect->SetOwner(Obj);
		neweffect->SetOffset(Vec2(0.f, 10.f));
		CurLevel->AddObject(neweffect, LAYER_TYPE::EFFECT);

	}
	else if (UNIT_TYPE::MACHINE == stat.Type)
	{
		int demage = m_MDemage - stat.Armor;
		if (demage > 0)
			stat.HP -= demage;
	}

	DeleteObject(this);

	// 이미 계산 처리된 Bullet이면 다음 충돌체와 처리를 하지 말아야함
	m_Hit = true;
}



void CBullet::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (L"Body Collider" == _OtherCollider->GetName() && !m_Hit)
	{
		// 대상은 유닛이어야 함
		// 만일 유닛이 아닌 다른걸 건들면 잘못된 타겟
		CUnit* Target = dynamic_cast<CUnit*>(_OtherObj);
		if (nullptr == Target)
			assert(Target);


		if(Target->GetStat().HP > 0.f)
			Demagecall(Target);
	}

	else if (L"Range Collider" == _OtherCollider->GetName())
	{
		int a = 0;
	}
}

void CBullet::SetDir(Vec2 _Dir)
{
	m_Dir = _Dir;


	//// 앞에 위치
	// x축은 0보다크고 y축은 -0.5보다 크고 0.5보다 작을 시
	if (0 < m_Dir.x && (-0.2 <= m_Dir.y && 0.2 >= m_Dir.y))
	{
		m_Texture = m_bulletTex1;
	}
	// 위에 위치
	// y축은 0보다크고 x축은 -0.5보다 크고 0.5보다 작을 시
	else if (0 <= m_Dir.y && (-0.8 <= m_Dir.x && 0.8 >= m_Dir.x))
	{
		m_Texture = m_bulletTex2;
	}
	// 뒤에 위치
	// x축은 0보다작고 y축은 -0.5보다 크고 0.5보다 작을 시
	else if (0 > m_Dir.x && (-0.2 <= m_Dir.y && 0.2 >= m_Dir.y))
	{
		m_Texture = m_bulletTex3;
	}
	// 아래에 위치
	// y축은 0보다작고 y축은 -0.5보다 크고 0.5보다 작을 시
	else if (0 >= m_Dir.y && (-0.8 <= m_Dir.x && 0.8 >= m_Dir.x))
	{
		m_Texture = m_bulletTex4;
	}
}
