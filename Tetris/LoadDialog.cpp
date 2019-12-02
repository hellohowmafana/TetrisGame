#include "LoadDialog.hpp"
#include "resource.h"
#include "Configuration.hpp"

INT_PTR LoadDialog::LoadDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			wstring pathArchives(Configuration::singleton.pathArchives);
			DlgDirList(hDlg, (LPWSTR)pathArchives.c_str(), IDC_ARCHIVELIST, 0, 0);
			return (INT_PTR)TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			HWND hlbArchive = GetDlgItem(hDlg, IDC_ARCHIVELIST);
			DlgDirSelectEx(hDlg, singleton.szArchive, MAX_ARCHIVE_NAME, IDC_ARCHIVELIST);
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

wchar_t* LoadDialog::GetSelectedArchive()
{
	return szArchive;
}

LoadDialog LoadDialog::singleton;
