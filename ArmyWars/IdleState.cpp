#include "pch.h"
#include "IdleState.h"


CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	LOG(LOG_LEVEL::WARNING, L"Enter IdleState");
}

void CIdleState::FinalTick()
{

}

void CIdleState::Exit()
{
	LOG(LOG_LEVEL::WARNING, L"Exit IdleState");
}
