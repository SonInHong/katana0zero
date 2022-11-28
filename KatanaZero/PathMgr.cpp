#include "pch.h"
#include "PathMgr.h"
#include "CCore.h"

PathMgr::PathMgr()
	:m_szContentPath{}
{
}

PathMgr::~PathMgr()
{
}

void PathMgr::Initialize()
{
	GetCurrentDirectory(255, m_szContentPath);
	int i = 0;
	for (i = 0; m_szContentPath[i] != 0; ++i)
	{

	}

	while (m_szContentPath[i] != L'\\')
	{
		m_szContentPath[i] = 0;
		--i;
	}

	wcscat_s(m_szContentPath, 255, L"bin");

	SetWindowText(CCore::Create()->GetWindowData().hwnd, m_szContentPath);
}