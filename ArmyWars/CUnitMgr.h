#pragma once

class CUnit;
class CSkill;

class CUnitMgr
{
	SINGLE(CUnitMgr)

private:
	CTexture* m_CooltimeTex;
	CUnit*	m_PUnitVec[(int)PLAYER_UNIT_NAME::END];
	CUnit* m_EUnitVec[(int)ENEMY_UNIT_NAME::END];
	CSkill* m_SkillVec[(int)SKILL_NAME::END];

	vector<CUnit*> m_PSummonIndx;
	vector<CUnit*> m_ESummonIndx;

	int m_PKillCountList[(int)PLAYER_UNIT_NAME::END];
	int m_EKillCountList[(int)ENEMY_UNIT_NAME::END];

public:
	void Init();
	void Tick();

	void PUnitInit();
	void EUnitInit();
	void SkillInit();
	void SetPSummonIndx(PLAYER_UNIT_NAME _Name);
	void DeletePSummonIndx(PLAYER_UNIT_NAME _Name);

	void SetESummonIndx(ENEMY_UNIT_NAME _Name);
	void ClearESummonIndx();
public:
	CUnit* GetPUnit(PLAYER_UNIT_NAME _Name) { return m_PUnitVec[(int)_Name]; }
	CUnit* GetEUnit(ENEMY_UNIT_NAME _Name) { return m_EUnitVec[(int)_Name]; }
	vector<CUnit*>& GetPSummonIndx();
	vector<CUnit*>& GetESummonIndx();
	CUnit* GetPlayerUnit(PLAYER_UNIT_NAME _Name) { return m_PUnitVec[(int)_Name]; }
	CUnit* SearchPUnitByName(PLAYER_UNIT_NAME _Name);

	CSkill* GetSkill(SKILL_NAME _Skill) { return m_SkillVec[(int)_Skill]; }

	void IncreaseEkillCount(ENEMY_UNIT_NAME _name) { m_EKillCountList[(int)_name] += 1; }
	int GetEKillCount(ENEMY_UNIT_NAME _name) { return m_EKillCountList[(int)_name]; }

	CTexture* GetColltimeTex() { return m_CooltimeTex; }

};

