// Tetris.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Tetris.h"
#include "Configuration.h"
#include "Drawer.h"

#include <Unknwn.h>
#include <Windows.h>
#include <gdiplus.h>
#include "GameFrame.h"
#include "Controller.h"
#include "ArchiveDialog.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// gdiplus
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

    MSG msg;

    // Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		//Game_Main();
	}

	GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TETRIS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);

   Configuration* pConfiguration = &Configuration::singleton;
   if (!pConfiguration->Initialize())
   {
	   MessageBox(hWnd, _T("Load configuration failed."), NULL, MB_OK);
	   PostQuitMessage(0);
   }

   GameFrame* pGameFrame = &GameFrame::singleton;
   pGameFrame->Initialize(pConfiguration);
   PromptFrame* pPromptFrame = &PromptFrame::singleton;
   pPromptFrame->Initialize(pConfiguration);
   pGameFrame->SetPromptFrame(pPromptFrame);
   InfoFrame* pInfoFrame = &InfoFrame::singleton;
   pInfoFrame->Initialize(pConfiguration);
   pGameFrame->SetInfoFrame(pInfoFrame);
   pGameFrame->InitializeGame();

   Background* pBackground = &Background::singleton;
   pBackground->Initialize(pConfiguration);

   Drawer* pDrawer = &Drawer::singleton;
   pDrawer->Initialize(pGameFrame, pPromptFrame, pInfoFrame, pBackground);

   Controller* pController = &Controller::singleton;
   pController->Initialize(pConfiguration);
   pController->SetHWnd(hWnd);
   pController->SetGameFrame(pGameFrame);
   pController->Start();

   InvalidateRect(hWnd, NULL, FALSE);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_SAVE:
				//Controller::singleton.SaveGame();
				break;
			case ID_LOAD:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ARCHIVEDIALOG), hWnd, ArchiveDialog::ArchiveDialogProc);
				Controller::singleton.LoadGame(ArchiveDialog::singleton.GetSelectedArchive());
				InvalidateRect(hWnd, NULL, FALSE);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
			if (!Controller::singleton.IsInitialized()) break;

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			Drawer* pDrawer = &Drawer::singleton;
			pDrawer->AttachDC(hdc);
			pDrawer->DrawElements();
			pDrawer->DetachDC();

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_KEYDOWN:
		{
			Controller* pController = &Controller::singleton;
			switch (wParam)
			{
				case VK_LEFT:
					pController->StepHorizontal(true);
					break;
				case VK_RIGHT:
					pController->StepHorizontal(false);
					break;
				case VK_DOWN:
					pController->StepDown();
					break;
				case VK_UP:
					pController->Rotate();
					break;
				case VK_SPACE:
					pController->Drop();
					break;
				case VK_RETURN:
					pController->Restart();
					break;
				default:
					break;
			}
			InvalidateRect(hWnd, NULL, FALSE);
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

// Message handler for about box.
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
