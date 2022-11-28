#include "pch.h"
#include "CPanelUI.h"
#include "KeyMgr.h"

CPanelUI::CPanelUI()
	:dragStart{}
	, Tapped(false)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Tap()
{
	dragStart = KeyMgr::Create()->GetMousePos();
	ScreenToClient(m_WindowData.hwnd, &dragStart);

	Tapped = true;
}

void CPanelUI::Press()
{
	if (Tapped == true)
	{
		POINT curpos = KeyMgr::Create()->GetMousePos();
		ScreenToClient(m_WindowData.hwnd, &curpos);


		setOffset(doublepoint{ (double)(curpos.x - dragStart.x), (double)(curpos.y - dragStart.y) } + m_Offset);

		dragStart = curpos;
	}

}

void CPanelUI::Release()
{
	Tapped = false;
}

void CPanelUI::Click()
{
}
