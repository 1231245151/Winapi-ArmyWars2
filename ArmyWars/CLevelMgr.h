#pragma once



class CLevel;
class CSound;

class CLevelMgr
{
	SINGLE(CLevelMgr);
private:
	CLevel* m_arrLevel[(int)LEVEL_TYPE::END];	// 모든 레벨
	CLevel* m_CurLevel; // 현재 지정된 레벨
	bool    m_Change;
	LEVEL_TYPE m_LevelLayer;

	LEVEL_TYPE m_StageSelectLayer;

	CLevel* m_PrevLevel;
	LEVEL_TYPE m_PrevLayer;

	CSound* m_GameBGM;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	CLevel* GetPrevLevel() { return m_PrevLevel; }
	LEVEL_TYPE GetPrevLayer() { return m_PrevLayer; }

	void SetStageSelectLayer(LEVEL_TYPE _Level) { m_StageSelectLayer = _Level; }
	LEVEL_TYPE GetStageSelectLayer() { return m_StageSelectLayer; }

public:
	void Init();
	void Tick();
	void Render();

	void PlayGameBGM(bool _continue = false);
	void StopGameBGM(bool _bReset);
	void SetGameBGM(CSound* _BGM);
	CSound* GetGameBGM() { return m_GameBGM; }

private:
	void ChangeLevel(LEVEL_TYPE _Type);
	void CameraChange();
	friend class CTaskMgr;
};


