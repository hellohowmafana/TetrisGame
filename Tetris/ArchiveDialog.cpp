#include "ArchiveDialog.h"
#include "resource.h"
#include "Configuration.h"

INT_PTR ArchiveDialog::ArchiveDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			wstring pathArchives(Configuration::singleton.pathArchives);
			DlgDirList(hDlg, (LPWSTR)pathArchives.c_str(), IDC_ARCHIVELIST, 0, 0);
			SetFocus(GetDlgItem(hDlg, IDC_ARCHIVELIST));
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

wchar_t* ArchiveDialog::GetSelectedArchive()
{
	return szArchive;
}

ArchiveDialog ArchiveDialog::singleton;
