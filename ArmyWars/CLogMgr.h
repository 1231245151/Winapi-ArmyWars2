#pragma once


class CLogMgr
{
	SINGLE(CLogMgr);
private:
	list<tLog>	m_LogList;		// 쌓인 로그
	float		m_LogMaxLife;	// 각 로그의 라이프타임
	UINT		m_LogStep;		// 로그 간격

	bool m_LogOnOff;

public:
	void SetLogMaxLife(float _Life) { m_LogMaxLife = _Life; }
	void SetLogStep(UINT _Step) { m_LogStep = _Step; }
	void AddLog(const tLog& _Log)
	{
#ifdef _DEBUG
		m_LogList.push_front(_Log);
#endif
	};

public:
	void Tick();
};

#define LOG(Level, Msg) {\
							wstring FuncName;\
							string str(__FUNCTION__);\
							FuncName = wstring(str.begin(), str.end());\
							wchar_t szBuff[255] = {};\
							swprintf_s(szBuff, 255, L"FunctionName : %s, { Line : %d }, %s", FuncName.c_str(), __LINE__, Msg);\
							CLogMgr::Get()->AddLog(tLog{ szBuff, Level, 0.f });\
						}




