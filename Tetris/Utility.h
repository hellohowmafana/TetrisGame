#pragma once
#include <random>
#include <Windows.h>
using namespace std;
#pragma comment (lib,"Imm32.lib")

class Utility
{
public:
	static int Random(int min, int max);
	static bool RandomTrue(double rate);

	static int Spliwstring(wchar_t* szString, wchar_t delimiter, wchar_t** tokens, int count);
	static COLORREF LightColor(COLORREF color, double percent);

	static void CenterWindow(HWND hWnd);
	static void MoveWindow(HWND hWnd, int x, int y);
	static void ResizeWindow(HWND hWnd, int width, int height);
	static void ResizableWindow(HWND hWnd, bool resizable);

	static HIMC hImc;
	static void DisableIMM(HWND hWnd);
	static void EnableIMM(HWND hWnd);
};

