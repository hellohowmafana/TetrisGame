#pragma once
#include <Windows.h>

#define MAX_ARCHIVE_NAME 32

class LoadDialog
{
public:
	static INT_PTR CALLBACK LoadDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	wchar_t* GetSelectedArchive();

	static LoadDialog singleton;

private:
	LoadDialog() {};

	wchar_t szArchive[MAX_ARCHIVE_NAME];
};

