#pragma once
#include <Windows.h>

#define MAX_RECORD_NAME 32

class ReplayDialog
{
public:
	static INT_PTR CALLBACK ReplayDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	wchar_t* GetSelectedRecord();

	static ReplayDialog singleton;

private:
	ReplayDialog() {};

	wchar_t szRecord[MAX_RECORD_NAME];
};

