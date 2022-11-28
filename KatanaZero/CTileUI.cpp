#include "pch.h"
#include "CTileUI.h"
#include "CResourceMgr.h"
#include "CCore.h"
#include "CScene_Tool.h"
#include "CSceneMgr.h"
#include "PenMgr.h"
#include "KeyMgr.h"

int CTileUI::posx = 0;
int CTileUI::posy = 0;

CTileUI::CTileUI(POINT scale)
	:relativePath{}
	,clicked(false)
{
	Texture = nullptr;
	Scale.x = scale.x;
	Scale.y = scale.y;
	m_Offset.x = posx;
	m_Offset.y = posy;

	posx += scale.x + 1;
}

CTileUI::CTileUI()
	:relativePath{}
	, clicked(false)
{
}

CTileUI::CTileUI(std::wstring _Path)
	:relativePath(_Path)
	,clicked(false)
{

	Texture = CResourceMgr::Create()->Load<CTexture>(_Path);
	Scale.x = Texture->GetWidth();
	Scale.y = Texture->GetHeight();
	m_Offset.x = posx;
	m_Offset.y = posy;

	

	posx += Texture->GetWidth() +1;
	
	if (posx + Texture->GetWidth() > CCore::Create()->GetToolWindowData().width)
	{
		posx = 0;
		posy += Texture->GetHeight() + 1;
	}
}

CTileUI::~CTileUI()
{
}

void CTileUI::Render(HDC _dc)
{
	if(Texture)
		BitBlt(m_dc, Pos.x, Pos.y, Scale.x, Scale.y, Texture->GetDC(), 0, 0, SRCCOPY);
	
	//HPEN oldPen = nullptr;

	if (clicked == true)
	{		
		HPEN oldPen = (HPEN)SelectObject(m_dc, PenMgr::Create()->GetGreenPen());
		HBRUSH oldBrush = (HBRUSH)SelectObject(m_dc, GetStockObject(NULL_BRUSH));
		Rectangle(m_dc, Pos.x, Pos.y, Pos.x + Scale.x, Pos.y + Scale.y);
		SelectObject(m_dc, oldBrush);
		SelectObject(m_dc, oldPen);
	}

	
	else
	{
		HBRUSH oldBrush = (HBRUSH)SelectObject(m_dc, GetStockObject(NULL_BRUSH));
		Rectangle(m_dc, Pos.x, Pos.y, Pos.x + Scale.x, Pos.y + Scale.y);
		SelectObject(m_dc, oldBrush);
	}
	
}

void CTileUI::Tap()
{
	if (KeyMgr::Create()->key(Key::LCTRL).pressed)
	{
		dragStart = KeyMgr::Create()->GetMousePos();
		ScreenToClient(m_WindowData.hwnd, &dragStart);

		Tapped = true;
	}
	
}

void CTileUI::Press()
{
	if (Tapped == true)
	{
		POINT curpos = KeyMgr::Create()->GetMousePos();
		ScreenToClient(m_WindowData.hwnd, &curpos);


		setOffset(doublepoint{ (double)(curpos.x - dragStart.x), (double)(curpos.y - dragStart.y) } + m_Offset);

		dragStart = curpos;
	}
}

void CTileUI::Release()
{
	Tapped = false;
}

void CTileUI::Click()
{
	CScene_Tool* toolscene = dynamic_cast<CScene_Tool*>(CSceneMgr::Create()->GetCurScene());
	if (toolscene)
		toolscene->SetBrushTile(relativePath);

	clicked = true;

}
