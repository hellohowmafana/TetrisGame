#include "ReplayDialog.hpp"
#include "resource.h"
#include "Configuration.hpp"

INT_PTR ReplayDialog::ReplayDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wstring pathRecords(Configuration::singleton.pathRecords);
		DlgDirList(hDlg, (LPWSTR)pathRecords.c_str(), IDC_RECORDLIST, 0, 0);
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			HWND hlbRecord = GetDlgItem(hDlg, IDC_RECORDLIST);
			DlgDirSelectEx(hDlg, singleton.szRecord, MAX_RECORD_NAME, IDC_RECORDLIST);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

wchar_t* ReplayDialog::GetSelectedRecord()
{
	return szRecord;
}

ReplayDialog ReplayDialog::singleton;
