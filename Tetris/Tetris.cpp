// Tetris.cpp : Defines the entry point for the application.
//
#include "framework.h"

#include <Unknwn.h>
#include <Windows.h>
#include <gdiplus.h>

#include "Tetris.hpp"
#include "Configuration.hpp"
#include "Drawer.hpp"
#include "GameFrame.hpp"
#include "Controller.hpp"
#include "SaveDialog.hpp"
#include "LoadDialog.hpp"
#include "SettingInputDialog.hpp"
#include "SettingGameDialog.hpp"
#include "Utility.hpp"
#include "Helper.hpp"
#include "Musician.hpp"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
wchar_t szTitle[MAX_LOADSTRING];                  // The title bar text
wchar_t szWindowClass[MAX_LOADSTRING];            // the main window class name

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

   Utility::DisableIMM(hWnd);

   Configuration* pConfiguration = &Configuration::singleton;
   if (!pConfiguration->Initialize())
   {
	   MessageBox(hWnd, L"Load configuration failed.", NULL, MB_OK);
	   PostQuitMessage(0);
   }
   
   Helper::AdjustWindow(hWnd, pConfiguration);

   PromptFrame* pPromptFrame = &PromptFrame::singleton;
   InfoFrame* pInfoFrame = &InfoFrame::singleton;
   GameFrame* pGameFrame = &GameFrame::singleton;
   pPromptFrame->SetGameFrame(pGameFrame);
   pPromptFrame->Initialize(pConfiguration);
   pInfoFrame->SetGameFrame(pGameFrame);
   pInfoFrame->Initialize(pConfiguration);
   pGameFrame->SetPromptFrame(pPromptFrame);
   pGameFrame->SetInfoFrame(pInfoFrame);
   pGameFrame->Initialize(pConfiguration);
   pGameFrame->InitializeGame();

   Background* pBackground = &Background::singleton;
   pBackground->Initialize(pConfiguration);
   
   Controller* pController = &Controller::singleton;

   Drawer* pDrawer = &Drawer::singleton;
   pDrawer->SetHWnd(hWnd);
   pDrawer->Initialize(pController, pGameFrame, pPromptFrame, pInfoFrame, pBackground);

   Musician* pMusician = &Musician::singleton;
   pMusician->InitializeInNewThread(pConfiguration);

   pController->Initialize(pConfiguration);
   pController->SetHWnd(hWnd);
   pController->SetGameFrame(pGameFrame);
   pController->SetDrawer(pDrawer);
   pController->SetMusician(pMusician);
   pController->Start();

   HMENU hMenu = GetMenu(hWnd);
   CheckMenuItem(hMenu, ID_MUSIC_BGM, pController->GetBgmOn() ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(hMenu, ID_MUSIC_SOUND, pController->GetSoundOn() ? MF_CHECKED : MF_UNCHECKED);

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
				SaveDialog saveDialog;
				if (IDOK == DialogBox(hInst, MAKEINTRESOURCE(IDD_SAVE), hWnd, SaveDialog::DialogProc))
				{
					wstring archive = saveDialog.GetArchiveName();
					if (Archive::Exist(archive))
						if (IDNO == MessageBox(hWnd, L"File exists, overwrite it?", L"Save", MB_YESNO))
							break;
					Controller::singleton.SaveGame(archive);
				}
				break;
			case ID_LOAD:
				LoadDialog loadDialog;
				if (IDOK == DialogBox(hInst, MAKEINTRESOURCE(IDD_LOAD), hWnd, LoadDialog::DialogProc))
				{
					Controller::singleton.LoadGame(loadDialog.GetSelectedArchive());
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_MUSIC_BGM:
			{
				Controller* pController = &Controller::singleton;
				bool bgmOn = !pController->GetBgmOn();
				pController->SetBgmOn(bgmOn);
				HMENU hMenu = GetMenu(hWnd);
				CheckMenuItem(hMenu, ID_MUSIC_BGM, bgmOn ? MF_CHECKED : MF_UNCHECKED);
				if (bgmOn)
					pController->PlayBgm();
				else
					pController->StopBgm();
				Configuration::singleton.SaveBgmOn(bgmOn);
				break;
			}
			case ID_MUSIC_SOUND:
			{
				Controller* pController = &Controller::singleton;
				bool soundOn = !pController->GetSoundOn();
				pController->SetSoundOn(soundOn);
				HMENU hMenu = GetMenu(hWnd);
				CheckMenuItem(hMenu, ID_MUSIC_SOUND, soundOn ? MF_CHECKED : MF_UNCHECKED);
				Configuration::singleton.SaveSoundOn(soundOn);
				break;
			}
			case ID_SETTING_INPUT:
			{
				if (IDOK == DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGINPUT), hWnd, SettingInputDialog::DialogProc))
				{
					SettingInputDialog::Setting* pSetting = &SettingInputDialog::setting;
					Configuration* pConfiguration = &Configuration::singleton;
					pConfiguration->LoadInput(pSetting->left, pSetting->right, pSetting->down,
						pSetting->drop, pSetting->rotate, pSetting->pause, pSetting->restart);
					Controller* pController = &Controller::singleton;
					pController->UpdateInputSettings(pConfiguration);
					pConfiguration->SaveInput();
				}
				break;
			}
			case ID_SETTING_GAME:
			{
				if (IDOK == DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGGAME), hWnd, SettingGameDialog::DialogProc))
				{
					Configuration* pConfiguration = &Configuration::singleton;
					bool sizeChanged = pConfiguration->frameSizeX != SettingGameDialog::setting.col ||
						pConfiguration->frameSizeY != SettingGameDialog::setting.row;
					if (sizeChanged)
					{
						pConfiguration->LoadPreconfiguration(SettingGameDialog::setting.row, SettingGameDialog::setting.col);
						pConfiguration->SavePositions();
					}
					if (pConfiguration->startLevel != SettingGameDialog::setting.level)
					{
						pConfiguration->startLevel = SettingGameDialog::setting.level;
						pConfiguration->SaveStartLevel(pConfiguration->startLevel);
					}
					if (pConfiguration->startLine != SettingGameDialog::setting.startLine)
					{
						pConfiguration->startLine = SettingGameDialog::setting.startLine;
						pConfiguration->SaveStartLine(pConfiguration->startLine);
					}
					GameFrame::singleton.Initialize(pConfiguration);
					if (sizeChanged)
					{
						PromptFrame::singleton.Initialize(pConfiguration);
						InfoFrame::singleton.Initialize(pConfiguration);
						Helper::AdjustWindow(hWnd, pConfiguration);
					}
					GameFrame::singleton.InitializeGame();
					Controller* pController = &Controller::singleton;
					pController->UpdateLevel();
					pController->End();
				}

				break;
			}
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
		break;
	}
	case WM_ERASEBKGND:
		return 1;
	case WM_KEYDOWN:
	{
		Controller* pController = &Controller::singleton;
		pController->OnKeyDown(wParam);
		break;
	}
	case WM_KEYUP:
	{
		Controller* pController = &Controller::singleton;
		pController->OnKeyUp(wParam);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseDown(VK_LBUTTON);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseDown(VK_MBUTTON);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseDown(VK_RBUTTON);
		break;
	}
	case WM_LBUTTONUP:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseUp(VK_LBUTTON);
		break;
	}
	case WM_MBUTTONUP:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseUp(VK_MBUTTON);
		break;
	}
	case WM_RBUTTONUP:
	{
		Controller* pController = &Controller::singleton;
		pController->OnMouseUp(VK_RBUTTON);
		break;
	}
	case WM_DESTROY:
		Drawer::singleton.Deinitialize();
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
