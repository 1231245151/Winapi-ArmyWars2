#include "pch.h"
#include "CBase.h"

UINT CBase::g_ID = 0;

CBase::CBase()
	: m_ID(g_ID++)
{
}

CBase::CBase(const CBase& _Other)
	: m_ID(g_ID++)
	, m_Name(_Other.m_Name)
{
}

CBase::~CBase()
{
}