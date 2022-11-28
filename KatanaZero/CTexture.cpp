#include "pch.h"
#include "CTexture.h"
#include "CCore.h"
#include "PathMgr.h"

CTexture::CTexture()
	:m_dc(0)
	, m_hBit(0)
{
}

CTexture::~CTexture()
{
}

bool CTexture::Load(std::wstring _strFilePath)
{
	std::wstring ContentPath = {};
	ContentPath += PathMgr::Create()->GetContentPath();
	ContentPath += L"\\content\\texture\\";
	ContentPath += _strFilePath;

	m_hBit = (HBITMAP)LoadImageW(nullptr, ContentPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (m_hBit)
	{
		m_dc = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);

		HBITMAP used = (HBITMAP)SelectObject(m_dc, m_hBit);
		DeleteObject(used);

		BITMAP bitinfo = {};

		GetObject(m_hBit, sizeof(BITMAP), &bitinfo);

		height = bitinfo.bmHeight;
		width = bitinfo.bmWidth;

		return true;
	}

	else
		return false;


}

void CTexture::MakeBlankTexture(int _width, int _height)
{
	width = _width;
	height = _height;

	m_hBit = CreateCompatibleBitmap(CCore::Create()->GetWindowData().hdc, _width, _height);
	m_dc = CreateCompatibleDC(CCore::Create()->GetWindowData().hdc);

	HBITMAP temp = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(temp);

	Rectangle(m_dc, -1, -1, _width + 1, _height + 1);


	
}
