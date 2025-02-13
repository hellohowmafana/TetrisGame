#pragma once
#include <Windows.h>

#define MAX_ARCHIVE_NAME 32

class LoadDialog
{
public:
	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static wchar_t* GetSelectedArchive();

private:
	static wchar_t szArchive[MAX_ARCHIVE_NAME];
};

