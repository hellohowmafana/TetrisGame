#pragma once
#include <Windows.h>
#include <tchar.h>

#define MAX_ARCHIVE_NAME 32

class ArchiveDialog
{
public:
	static INT_PTR CALLBACK ArchiveDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	TCHAR* GetSelectedArchive();

	static ArchiveDialog singleton;

private:
	ArchiveDialog() {};

	TCHAR szArchive[MAX_ARCHIVE_NAME];
};

