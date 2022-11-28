#include "pch.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CCore.h"
#include "CScene.h"
#include "CObject.h"
#include "CUI.h"
#include "KeyMgr.h"
#include "CTileUI.h"


CUIMgr::CUIMgr()
	:pressed(false)
	, pressedWhere(nullptr)
	, FocusUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::Update()
{
	//���� ������ UI �����ͼ�
	std::vector<CObject*>& Cur_UI = CSceneMgr::Create()->GetCurScene()->GetUI();

	//��Ŀ�� ����
	std::vector<CObject*>::iterator focus = Cur_UI.end();

	if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::TAP)
	{
		std::vector<CObject*>::iterator iter = Cur_UI.begin();

		while (iter != Cur_UI.end())
		{
			if (((CUI*)(*iter))->IsMouseOn())
				focus = iter;

			++iter;
		}

		if (focus != Cur_UI.end())
		{
			FocusUI = (CUI*)(*focus);
			Cur_UI.erase(focus);
			Cur_UI.push_back(FocusUI);
		}

	}

	//���콺 �µ� UI ����
	CUI* targetUI = nullptr;

	targetUI = WhoAmIPointing(FocusUI);


	if (targetUI != nullptr)
	{
		if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::TAP)
		{
			targetUI->Tap();
			pressed = true;
			pressedWhere = targetUI;

		}

		if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::PRESS)
			targetUI->Press();

		if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::RELEASE)
		{
			targetUI->Release();

			if (pressedWhere == targetUI)
			{
				static int a = 0;
				for (int i = 0; i < Cur_UI.size(); ++i)
				{
					CTileUI* t = dynamic_cast<CTileUI*>(Cur_UI[i]);
					if (t)
						t->SetClicked(false);
				}

				

				
				targetUI->Click();
			}
				
		}

	}

	if (KeyMgr::Create()->key(Key::LEFTMOUSE).key_state == KeyState::RELEASE)
	{
		pressed = false;
		pressedWhere = nullptr;
	}

}

CUI* CUIMgr::WhoAmIPointing(CUI* _input)
{
	if (_input == nullptr)
		return nullptr;

	CUI* target = nullptr;
	const std::vector<CUI*>& childs = _input->GetChilds();

	for (int i = 0; i < childs.size(); ++i)
		target = WhoAmIPointing(childs[i]);

	if (target == nullptr && _input->IsMouseOn())
		target = _input;

	return target;
}
