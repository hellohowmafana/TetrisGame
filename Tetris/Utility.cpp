#include "Utility.h"

int Utility::Random(int min, int max) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(min, max);
	return dist(mt);
};

bool Utility::RandomTrue(double rate)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(mt) < rate;
}

int Utility::Spliwstring(wchar_t* szString, wchar_t delimiter, wchar_t** tokens, int count)
{
	// return tokens count
	if (nullptr == tokens)
	{
		count = 0;
		wchar_t* pc = wcschr(szString, delimiter);
		count++;
		while (pc != NULL)
		{
			pc = wcschr(pc + 1, delimiter);
			count++;
		}
		return count;
	}
	// 0 for maximum
	wchar_t szDelimiters[2] = { delimiter, 0 };
	wchar_t* pwc;
	wchar_t* ptr = wcstok(szString, szDelimiters, &pwc);
	bool hasCount = 0 != count;
	int leftCount = count;
	while (ptr != NULL && (hasCount ? leftCount : true))
	{
		*tokens++ = ptr;
		ptr = wcstok(NULL, szDelimiters, &pwc);
		leftCount--;
	}
	return count - leftCount;
}

void Utility::CenterWindow(HWND hWnd)
{
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int x = (screenX - (rect.right - rect.left)) / 2;
	int y = (screenY - (rect.bottom - rect.top)) / 2;
	SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Utility::MoveWindow(HWND hWnd, int x, int y)
{
	SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Utility::ResizeWindow(HWND hWnd, int width, int height)
{
	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Utility::ResizableWindow(HWND hWnd, bool resizable)
{
	LONG_PTR style = resizable ?
		GetWindowLongPtr(hWnd, GWL_STYLE) | WS_THICKFRAME :
		GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_THICKFRAME;
	SetWindowLongPtr(hWnd, GWL_STYLE, style);
}

HIMC Utility::hImc;

void Utility::DisableIMM(HWND hWnd)
{
	if (hImc)
	{
		ImmDestroyContext(hImc);
		hImc = NULL;
	}
	ImmAssociateContext(hWnd, hImc);
}

void Utility::EnableIMM(HWND hWnd)
{
	if (!hImc)
		hImc = ImmCreateContext();
	ImmAssociateContext(hWnd, hImc);
}
