#include "pch.h"
#include "CCore.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CSceneMgr.h"
#include "PathMgr.h"
#include "CColliderMgr.h"
#include "CCameraMgr.h"
#include "framework.h"
#include <Windows.h>
#include "Resource.h"
#include "CUIMgr.h"
#include "PenMgr.h"


CCore::CCore()
	:A_WindowData{}
	, mem_dc{}
	, mem_bitmap{}
	, mem_menu{}
	, mem_toolmenu{}
{
}

CCore::~CCore()
{
}

void CCore::Initialize(WindowData windowdata, WindowData _ToolwindowData)
{
	mem_menu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_KATANAZERO));
	mem_toolmenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_TOOLMENU));

	A_WindowData = windowdata;
	A_ToolWindowData = _ToolwindowData;

	RECT rt = { 0,0,A_WindowData.width,A_WindowData.height };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(A_WindowData.hwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(A_WindowData.hwnd, SW_NORMAL);

	A_WindowData.hdc = GetDC(A_WindowData.hwnd);
	A_ToolWindowData.hdc = GetDC(A_ToolWindowData.hwnd);

	mem_dc = CreateCompatibleDC(A_WindowData.hdc);
	mem_bitmap = CreateCompatibleBitmap(A_WindowData.hdc, A_WindowData.width, A_WindowData.height);

	HBITMAP used = (HBITMAP)SelectObject(mem_dc, mem_bitmap);
	DeleteObject(used);

	PenMgr::Create()->Initialize();
	TimeMgr::Create()->Initialize();
	PathMgr::Create()->Initialize();
	KeyMgr::Create()->Initialize();
	CSceneMgr::Create()->Initialize();
	CColliderMgr::Create()->Initialize();



}

void CCore::Update()
{
	TimeMgr::Create()->Update();
	KeyMgr::Create()->Update();

	CCameraMgr::Create()->Update(); //카메라 업데이트
	CUIMgr::Create()->Update(); // UI 업데이트
	CSceneMgr::Create()->Update(); // 오브젝트 업데이트
	CSceneMgr::Create()->ComponentUpdate(); // 컴포넌트 업데이트
	CColliderMgr::Create()->Update(); // 충돌처리
	//CSceneMgr::Create()->Clean(); // 충돌후 삭제처리

}

void CCore::Render()
{
	CSceneMgr::Create()->Render(mem_dc);
}

void CCore::Destroy()
{
}

void CCore::SetMenuBar(bool power)
{
	if (power)
		SetMenu(A_WindowData.hwnd, mem_menu);

	else
		SetMenu(A_WindowData.hwnd, nullptr);

	RECT rt = { 0,0,A_WindowData.width,A_WindowData.height };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, power);
	SetWindowPos(A_WindowData.hwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(A_WindowData.hwnd, true);
}

void CCore::SetToolMenuBar(bool power)
{
	if (power)
		SetMenu(A_ToolWindowData.hwnd, mem_toolmenu);

	else
		SetMenu(A_ToolWindowData.hwnd, nullptr);

	RECT rt = { 0,0, A_ToolWindowData.width, A_ToolWindowData.height };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, power);
	SetWindowPos(A_ToolWindowData.hwnd, nullptr, A_WindowData.width+1, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(A_ToolWindowData.hwnd, true);
}


