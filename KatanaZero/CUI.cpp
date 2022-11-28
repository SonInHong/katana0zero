#include "pch.h"
#include "CUI.h"
#include "CCore.h"
#include "KeyMgr.h"

CUI::CUI()
	:m_Parent(nullptr)
	, m_Childs{}
	, m_Offset{}
	, m_WindowData{}
	,m_dc{}
{
}

CUI::~CUI()
{
}

void CUI::Initialize()
{

}

void CUI::Update()
{


	UpdatePosition();
}

void CUI::UpdatePosition()
{
	CUI* parent = GetParent();

	if (parent != nullptr)
		Pos = parent->GetPos() + m_Offset;

	else
		Pos = m_Offset;

	for (int i = 0; i < m_Childs.size(); ++i)
	{
		m_Childs[i]->UpdatePosition();
	}

}

void CUI::Render(HDC _dc)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));
	Rectangle(_dc, Pos.x, Pos.y, Pos.x + Scale.x, Pos.y + Scale.y);
	SelectObject(_dc, oldBrush);

	for (int i = 0; i < m_Childs.size(); ++i)
	{
		m_Childs[i]->Render(_dc);
	}
}

void CUI::Tap()
{

}

void CUI::Press()
{
}

void CUI::Release()
{

}

void CUI::Click()
{
	int a = 0;
}

bool CUI::IsMouseOn()
{
	POINT MousePos = KeyMgr::Create()->GetMousePos();
	ScreenToClient(m_WindowData.hwnd, &MousePos);

	if (MousePos.x <= Pos.x + Scale.x
		&& MousePos.x >= Pos.x
		&& MousePos.y <= Pos.y + Scale.y
		&& MousePos.y >= Pos.y)
		return true;

	else
		return false;

}

