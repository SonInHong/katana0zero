// KatanaZero.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Resource.h"
#include "pch.h"
#include "CCore.h"
#include "CEventMgr.h"



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
WCHAR szToolWindowClass[MAX_LOADSTRING];
WindowData g_windowdate, t_windowdate;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance, WNDPROC, LPCWSTR);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ToolWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ToolResetBox(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    //메모리 누수를 체크해주는 함수
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(숫자)

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KATANAZERO, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance, WndProc, szWindowClass);
    MyRegisterClass(hInstance, ToolWndProc, L"ToolWindow");

    // 애플리케이션 초기화를 수행합니다:


    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    //Application::Create()->Initialize(g_windowdate);






    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KATANAZERO));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (1)
    {


        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지가 발생했을때
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            // Application::Create()->Update();
             //Application::Create()->Render();
            CCore::Create()->Update();
            CCore::Create()->Render();
            CEventMgr::Create()->Update();

        }
    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC _wndproc, LPCWSTR _title)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = _wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KATANAZERO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_SATELLITE);
    wcex.lpszClassName = _title;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    g_windowdate.clear(); //메인 윈도우 생성
    g_windowdate.height = 1080;
    g_windowdate.width = 1920;


    g_windowdate.hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_windowdate.hwnd)
    {
        return FALSE;
    }

    ShowWindow(g_windowdate.hwnd, nCmdShow);
    UpdateWindow(g_windowdate.hwnd);



    t_windowdate.clear(); // 툴 윈도우 생성
    t_windowdate.height = 300;
    t_windowdate.width = 300;

    t_windowdate.hwnd = CreateWindowW(L"ToolWindow", szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!t_windowdate.hwnd)
    {
        return FALSE;
    }

    SetWindowPos(t_windowdate.hwnd, nullptr, 1600, 0, t_windowdate.width, t_windowdate.height, 0);
    //ShowWindow(t_windowdate.hwnd, nCmdShow);
    //ShowWindow(t_windowdate.hwnd, SW_HIDE);
    //UpdateWindow(t_windowdate.hwnd);



    CCore::Create()->Initialize(g_windowdate, t_windowdate);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

#include "KeyMgr.h"
#include "CScene_Tool.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case ID_SAVE:
        {
            CScene_Tool* toolscene = dynamic_cast<CScene_Tool*>(CSceneMgr::Create()->GetCurScene());
            if (toolscene)
                toolscene->Save();

            KeyMgr::Create()->SetModal(true);
        }
            break;

        case ID_LOAD:
        {
            CScene_Tool* toolscene = dynamic_cast<CScene_Tool*>(CSceneMgr::Create()->GetCurScene());
            if (toolscene)
                toolscene->Load();

            KeyMgr::Create()->SetModal(true);
        }
            break;

        case ID_Reset:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_TILECOUNT), hWnd, ToolResetBox);
        }
           break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;

    
}

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_ToolLoad:
        {
            CScene_Tool* toolscene = dynamic_cast<CScene_Tool*>(CSceneMgr::Create()->GetCurScene());
            if (toolscene)
                toolscene->LoadTile();

            KeyMgr::Create()->SetModal(true);
        }
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ToolResetBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            int x_C = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
            int y_S = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);
            int x_S = GetDlgItemInt(hDlg, IDC_EDIT3, nullptr, false);
            int y_C = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false);

            CScene_Tool* toolscene = dynamic_cast<CScene_Tool*>(CSceneMgr::Create()->GetCurScene());
            if (toolscene)
                toolscene->CreateTiles(POINT{ x_C,y_C }, POINT{ x_S,y_S });

            KeyMgr::Create()->SetModal(true);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDCANCEL)
        {
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}