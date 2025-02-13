#include "SettingInputDialog.hpp"
#include "resource.h"
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "Configuration.hpp"

LRESULT CALLBACK SettingInputDialog::EditSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    static HIMC hImc = nullptr;

    static HWND hWndLastClick = nullptr;
    unsigned char* pVirtualCode = nullptr;

    switch (GetDlgCtrlID(hWnd))
    {
    case IDE_LEFT:
        pVirtualCode = &SettingInputDialog::setting.left;
        break;
    case IDE_RIGHT:
        pVirtualCode = &SettingInputDialog::setting.right;
        break;
    case IDE_ROTATE:
        pVirtualCode = &SettingInputDialog::setting.rotate;
        break;
    case IDE_DOWN:
        pVirtualCode = &SettingInputDialog::setting.down;
        break;
	case IDE_DROP:
		pVirtualCode = &SettingInputDialog::setting.drop;
		break;
	case IDE_PAUSE:
		pVirtualCode = &SettingInputDialog::setting.pause;
		break;
	case IDE_RESTART:
		pVirtualCode = &SettingInputDialog::setting.restart;
		break;
    }

    auto handleMouseClick = [&](const wchar_t* buttonText) {
        if (hWndLastClick != hWnd)
        {
            hWndLastClick = hWnd;
            SetFocus(hWnd);
        }
        else
        {
            SetWindowText(hWnd, buttonText);
        }
    };

    switch (message)
    {
    case WM_CHAR:
        return 0;
    case WM_KEYDOWN:
    {
        wchar_t keyName[16];
        if(pVirtualCode)
            *pVirtualCode = wParam;
        GetKeyNameTextW(lParam, keyName, sizeof(keyName) / sizeof(wchar_t));
        SetWindowText(hWnd, keyName);
        return 0;
    }
    case WM_LBUTTONDOWN:
        wchar_t debugMessage[64];
        swprintf(debugMessage, sizeof(debugMessage) / sizeof(wchar_t), L"lbutond: HWND = 0x%p\n", hWnd);
        OutputDebugString(debugMessage);
        if (pVirtualCode)
            *pVirtualCode = VK_LBUTTON;
        handleMouseClick(L"LBUTTON");
        return 0;
    case WM_MBUTTONDOWN:
        if (pVirtualCode)
            *pVirtualCode = VK_MBUTTON;
        handleMouseClick(L"MBUTTON");
        return 0;
    case WM_RBUTTONDOWN:
        if (pVirtualCode)
            *pVirtualCode = VK_RBUTTON;
        handleMouseClick(L"RBUTTON");
        return 0;
    case WM_SETFOCUS:
        hImc = ImmAssociateContext(hWnd, nullptr);
        break;
    case WM_KILLFOCUS:
        ImmAssociateContext(hWnd, hImc);
        break;
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, EditSubclassProc, uIdSubclass);
        break;
    }
    return DefSubclassProc(hWnd, message, wParam, lParam);
}

wstring SettingInputDialog::GetInputText(unsigned char inputCode)
{
    switch (inputCode)
    {
	case VK_LBUTTON:
		return L"LBUTTON";
	case VK_MBUTTON:
		return L"MBUTTON";
	case VK_RBUTTON:
		return L"RBUTTON";
    default:
        UINT scanCode = MapVirtualKey(inputCode, MAPVK_VK_TO_VSC);
        wchar_t keyName[16];
        LONG lParam = (scanCode << 16);
        if (inputCode == VK_LEFT || inputCode == VK_RIGHT || inputCode == VK_UP || inputCode == VK_DOWN ||
            inputCode == VK_PRIOR || inputCode == VK_NEXT || inputCode == VK_END || inputCode == VK_HOME ||
            inputCode == VK_INSERT || inputCode == VK_DELETE || inputCode == VK_DIVIDE || inputCode == VK_NUMLOCK)
        {
            lParam |= (1 << 24); // Set the extended key flag
        }
        GetKeyNameTextW(lParam, keyName, sizeof(keyName) / sizeof(wchar_t));
        return wstring(keyName);
    }
}

INT_PTR SettingInputDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
        SetWindowSubclass(GetDlgItem(hDlg, IDE_LEFT), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_RIGHT), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_ROTATE), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_DOWN), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_DROP), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_PAUSE), EditSubclassProc, 0, 0);
        SetWindowSubclass(GetDlgItem(hDlg, IDE_RESTART), EditSubclassProc, 0, 0);
		setting.left = Configuration::singleton.actionLeft;
		setting.right = Configuration::singleton.actionRight;
        setting.rotate = Configuration::singleton.actionRotate;
		setting.down = Configuration::singleton.actionDown;
		setting.drop = Configuration::singleton.actionDrop;
		setting.pause = Configuration::singleton.operationPause;
		setting.restart = Configuration::singleton.operationRestart;
		SetDlgItemText(hDlg, IDE_LEFT, GetInputText(setting.left).c_str());
		SetDlgItemText(hDlg, IDE_RIGHT, GetInputText(setting.right).c_str());
		SetDlgItemText(hDlg, IDE_ROTATE, GetInputText(setting.rotate).c_str());
		SetDlgItemText(hDlg, IDE_DOWN, GetInputText(setting.down).c_str());
		SetDlgItemText(hDlg, IDE_DROP, GetInputText(setting.drop).c_str());
		SetDlgItemText(hDlg, IDE_PAUSE, GetInputText(setting.pause).c_str());
		SetDlgItemText(hDlg, IDE_RESTART, GetInputText(setting.restart).c_str());
        return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (BN_CLICKED == HIWORD(wParam))
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
			}
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

SettingInputDialog::Setting SettingInputDialog::setting;