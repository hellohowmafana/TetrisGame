#include "SettingGameDialog.hpp"
#include "resource.h"
#include "Commctrl.h"
#include "Configuration.hpp"

void SettingGameDialog::SetSize(HWND hDlg, int radioButtonId, int row, int col)
{
	if (radioButtonId != IDR_CUSTOM)
	{
		SendMessage(GetDlgItem(hDlg, IDS_ROW), TBM_SETPOS, TRUE, row);
		SendMessage(GetDlgItem(hDlg, IDS_COL), TBM_SETPOS, TRUE, col);
		SetDlgItemInt(hDlg, IDC_ROW, row, FALSE);
		SetDlgItemInt(hDlg, IDC_COL, col, FALSE);
	}
	CheckRadioButton(hDlg, IDR_SML, IDR_CUSTOM, radioButtonId);
	BOOL bEnable = (radioButtonId == IDR_CUSTOM);
	EnableWindow(GetDlgItem(hDlg, IDS_ROW), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDS_COL), bEnable);
}

INT_PTR SettingGameDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		setting.row = Configuration::singleton.frameSizeY;
		setting.col = Configuration::singleton.frameSizeX;

		HWND hwndRow, hwndCol;
		hwndRow = GetDlgItem(hDlg, IDS_ROW);
		hwndCol = GetDlgItem(hDlg, IDS_COL);
		SendMessage(hwndRow, TBM_SETRANGE, FALSE, MAKELONG(12, 30));
		SendMessage(hwndCol, TBM_SETRANGE, FALSE, MAKELONG(10, 50));
		SendMessage(hwndRow, TBM_SETPOS, TRUE, setting.row);
		SendMessage(hwndCol, TBM_SETPOS, TRUE, setting.col);
		SetDlgItemInt(hDlg, IDC_ROW, SendMessage(hwndRow, TBM_GETPOS, 0, 0), FALSE);
		SetDlgItemInt(hDlg, IDC_COL, SendMessage(hwndCol, TBM_GETPOS, 0, 0), FALSE);

		std::map<int, std::pair<int, int>>::const_iterator it;
		if ((it = predefinedSettings.find(IDR_SML)) != predefinedSettings.end()
			&& setting.row == it->second.first && setting.col == it->second.second)
		{
			SetSize(hDlg, IDR_SML, it->second.first, it->second.second);
		}
		else if ((it = predefinedSettings.find(IDR_MID)) != predefinedSettings.end()
			&& setting.row == it->second.first && setting.col == it->second.second)
		{
			SetSize(hDlg, IDR_MID, it->second.first, it->second.second);
		}
		else if ((it = predefinedSettings.find(IDR_BIG)) != predefinedSettings.end()
			&& setting.row == it->second.first && setting.col == it->second.second)
		{
			SetSize(hDlg, IDR_BIG, it->second.first, it->second.second);
		}
		else
		{
			SetSize(hDlg, IDR_CUSTOM, setting.row, setting.col);
		}

		SendMessage(GetDlgItem(hDlg, IDN_LEVEL), UDM_SETRANGE, 0, MAKELPARAM(Configuration::singleton.maxLevel, 1));
		SendMessage(GetDlgItem(hDlg, IDN_STARTLINE), UDM_SETRANGE, 0, MAKELPARAM(Configuration::singleton.frameSizeY, 0));
		SendMessage(GetDlgItem(hDlg, IDN_LEVEL), UDM_SETPOS32, 0, Configuration::singleton.startLevel);
		SendMessage(GetDlgItem(hDlg, IDN_STARTLINE), UDM_SETPOS32, 0, Configuration::singleton.startLine);

		return TRUE;
	}
	case WM_HSCROLL:
	{
		UINT idc;
		HWND hwnd = (HWND)lParam;
		if (hwnd == GetDlgItem(hDlg, IDS_ROW))
			idc = IDC_ROW;
		else if (hwnd == GetDlgItem(hDlg, IDS_COL))
			idc = IDC_COL;
		else
			return TRUE;

		UINT pos;
		if (TB_THUMBTRACK == LOWORD(wParam))
			pos = HIWORD(wParam);
		else
			pos = SendMessage(hwnd, TBM_GETPOS, 0, 0);
		SetDlgItemInt(hDlg, idc, pos, FALSE);
		return TRUE;
	}
	case WM_COMMAND:
	{
		if (BN_CLICKED == HIWORD(wParam))
		{
			switch (LOWORD(wParam))
			{
			case IDR_SML:
			case IDR_MID:
			case IDR_BIG:
			{
				auto it = predefinedSettings.find(LOWORD(wParam));
				if (it != predefinedSettings.end())
				{
					SetSize(hDlg, LOWORD(wParam), it->second.first, it->second.second);
				}
				break;
			}
			case IDR_CUSTOM:
				SetSize(hDlg, IDR_CUSTOM, setting.row, setting.col);
				break;
			case IDOK:
			{
				setting.row = GetDlgItemInt(hDlg, IDC_ROW, NULL, FALSE);
				setting.col = GetDlgItemInt(hDlg, IDC_COL, NULL, FALSE);
				setting.level = GetDlgItemInt(hDlg, IDE_LEVEL, NULL, FALSE);
				setting.startLine = GetDlgItemInt(hDlg, IDE_STARTLINE, NULL, FALSE);
				EndDialog(hDlg, LOWORD(wParam));
				break;
			}
			case IDCANCEL:
			{
				EndDialog(hDlg, LOWORD(wParam));
				break;
			}
			}
			return TRUE;
		}
	}
	}
	return (INT_PTR)FALSE;
}

SettingGameDialog::Setting SettingGameDialog::setting;

const std::map<int, std::pair<int, int>> SettingGameDialog::predefinedSettings = {
	{IDR_SML, {12, 10}},
	{IDR_MID, {24, 12}},
	{IDR_BIG, {26, 38}}
};