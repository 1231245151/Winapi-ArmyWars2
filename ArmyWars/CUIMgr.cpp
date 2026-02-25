#include "pch.h"
#include "CUIMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	:m_PreventFrame(0)
	, m_ActiveStat(true)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::Tick()
{
	if (m_PreventFrame)
	{
		--m_PreventFrame;
		return;
	}
	if (!m_ActiveStat)
	{
		return;
	}

	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LbtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// 현재 레벨을 확인
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// UI Layer 를 가져온다
	vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER_TYPE::UI);

	vector<CObj*>::reverse_iterator iter = vecUI.rbegin();

	vector<CUI*> pfirstUI;

	for (; iter != vecUI.rend();)
	{
		// 부모 UI 확인
		CUI* pUI = dynamic_cast<CUI*>((*iter));
		assert(pUI);

		// 부모 UI활성화 확인
		if (!(pUI->IsActive()))
		{
			StatusCheck(pUI);

			// 벡터에서 제일 앞으로 보낸다
			iter = std::make_reverse_iterator(vecUI.erase((iter + 1).base()));
			pfirstUI.push_back(pUI);

			continue;
		}

		// 부모UI 를 입력해서 실제 이벤트를 가져갈 우선순위 UI 를 알아냄
		CUI* pPriorityUI = GetPriorityUI(pUI);
		if (nullptr == pPriorityUI)
		{
			StatusCheck(pUI);
			++iter;
			continue;
		}

		if (LbtnState == KEY_STATE::TAP)
		{
			pPriorityUI->m_LbtnDown = true;
			pPriorityUI->LBtnDown();

			// 벡터에서 제일 뒤로 보낸다.
			vecUI.erase((iter + 1).base());
			vecUI.push_back(pUI);
			break;
		}

		if (LbtnState == KEY_STATE::RELEASED)
		{
			pPriorityUI->LBtnUp();
			if (pPriorityUI->m_LbtnDown)
			{
				pPriorityUI->LBtnClicked();
			}
			StatusCheck(pUI);
		}

		StatusCheck(pUI);
		break;
	}


	vecUI.insert(vecUI.begin(), pfirstUI.begin(), pfirstUI.end());
}

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI)
{
	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LbtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// 반환시킬 우선순위 UI 포인터
	CUI* pPriorityUI = nullptr;

	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모UI 입력
	queue.push_back(_ParentUI);

	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		// UI가 비활성화 상태면 해당 UI검사 x
		if (!(pUI->IsActive()))
		{
			const vector<CUI*>& vecChild = pUI->GetChildUI();
			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				queue.push_back(vecChild[i]);
			}
			continue;
		}
		// 우선순위 설정되어 있으면 해당 UI로 갱신
		if (pUI->m_Priority)
		{
			pPriorityUI = pUI;
			return pPriorityUI;
		}
		// 마우스가 해당 UI 위에 있었으면 PriorityUI 를 갱신한다.
		else if (pUI->m_MouseOn)
		{
			pPriorityUI = pUI;
		}

		// 마우스 On 관련 이벤트
		if (false == pUI->m_MouseOn_Prev && pUI->m_MouseOn)
			pUI->BeginHovered();
		else if (pUI->m_MouseOn_Prev && pUI->m_MouseOn)
			pUI->OnHovered();
		else if (pUI->m_MouseOn_Prev && !pUI->m_MouseOn)
			pUI->EndHovered();

		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

	return pPriorityUI;
}

void CUIMgr::StatusCheck(CUI* _ParentUI)
{
	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LbtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모UI 입력
	queue.push_back(_ParentUI);

	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// 왼쪽버튼이 해제된 상태면, 모든 UI 들은 눌림상태를 false 로 변경한다.
		if (LbtnState == KEY_STATE::RELEASED)
		{
			pUI->m_LbtnDown = false;
		}
	}
}