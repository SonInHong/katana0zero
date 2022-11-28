#include "pch.h"
#include "CCursor.h"
#include "CResourceMgr.h"
#include "KeyMgr.h"
#include "CCore.h"

CCursor::CCursor()
{
	Scale.x = 50;
	Scale.y = 50;
}

CCursor::~CCursor()
{
}

void CCursor::Initialize()
{
	Texture = CResourceMgr::Create()->Load<CTexture>(L"Cursor\\Cursor\\0.bmp");
}

void CCursor::Update()
{
	POINT mousepos = KeyMgr::Create()->GetMousePos();
	ScreenToClient(CCore::Create()->GetWindowData().hwnd, &mousepos);

	Pos.x = (double)mousepos.x;
	Pos.y = (double)mousepos.y;

}

void CCursor::Render(HDC _dc)
{
	TransparentBlt(_dc, Pos.x - Scale.x / 2, Pos.y - Scale.y / 2, Scale.x, Scale.y
		, Texture->GetDC(), 0,0, Texture->GetWidth(), Texture->GetHeight(), RGB(255, 255, 255));
}


