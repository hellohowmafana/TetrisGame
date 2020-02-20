#pragma once
#include <Windows.h>

#define MAX_RECORD_NAME 32

class PlayDialog
{
public:
	static INT_PTR CALLBACK PlayDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	wchar_t* GetSelectedRecord();

	static PlayDialog singleton;

private:
	PlayDialog() {};

	wchar_t szRecord[MAX_RECORD_NAME];
};

