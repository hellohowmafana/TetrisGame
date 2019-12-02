#pragma once
#include <Windows.h>

#define MAX_ARCHIVE_NAME 32

class SaveDialog
{
public:
	static INT_PTR CALLBACK SaveDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	wchar_t* GetArchiveName();

	static SaveDialog singleton;

private:
	SaveDialog() {};

	wchar_t szArchive[MAX_ARCHIVE_NAME];
};

