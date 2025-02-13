#pragma once
#include <Windows.h>
#include <map>

class SettingGameDialog
{
public:
	static void SetSize(HWND hDlg, int radioButtonId, int row, int col);
	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	struct Setting
	{
	public:
		UINT row;
		UINT col;
	};
	static Setting setting;

	static const std::map<int, std::pair<int, int>> predefinedSettings;
};
