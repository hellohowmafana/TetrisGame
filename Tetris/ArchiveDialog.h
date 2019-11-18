#pragma once
#include <Windows.h>

#define MAX_ARCHIVE_NAME 32

class ArchiveDialog
{
public:
	static INT_PTR CALLBACK ArchiveDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	wchar_t* GetSelectedArchive();

	static ArchiveDialog singleton;

private:
	ArchiveDialog() {};

	wchar_t szArchive[MAX_ARCHIVE_NAME];
};

