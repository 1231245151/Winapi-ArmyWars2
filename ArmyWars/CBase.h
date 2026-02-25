#pragma once



class CBase
{
private:
	static UINT g_ID;

private:
	wstring		m_Name; // wchar_t 타입 동적 배열
	const UINT	m_ID;	// Base 객체마다 고유한 값을 가짐

public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }
	UINT GetID() { return m_ID; }

public:
	virtual CBase* Clone() = 0;

public:
	CBase();
	CBase(const CBase& _Other);
	virtual ~CBase();
};

