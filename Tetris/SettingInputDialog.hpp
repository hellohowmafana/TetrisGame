#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class SettingInputDialog
{
private:
	static LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static wstring GetInputText(unsigned char inputCode);

public:
	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	struct Setting
	{
		unsigned char left;
		unsigned char right;
		unsigned char down;
		unsigned char rotate;
		unsigned char drop;
		unsigned char pause;
		unsigned char restart;
	};

	static Setting setting;
};

