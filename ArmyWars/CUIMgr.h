#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);
private:
	int		m_PreventFrame;
	bool    m_ActiveStat;

public:
	void Tick();

public:
	void SetPreventFrame(int _FrameCount)
	{
		m_PreventFrame = _FrameCount;
	}
	void SetActiveUI(bool _true) { m_ActiveStat = _true; }

private:
	CUI* GetPriorityUI(CUI* _ParentUI);
	void StatusCheck(CUI* _ParentUI);
};



