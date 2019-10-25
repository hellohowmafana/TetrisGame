#pragma once
#include <random>
#include <tchar.h>
#include <Windows.h>
using namespace std;

class Utility
{
public:
	static int Random(int min, int max) {
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(min, max);
		return dist(mt);
	};

	static bool RandomTrue(double rate)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0.0, 1.0);
		return dist(mt) < rate;
	}

	static int SplitString(TCHAR* szString, TCHAR delimiter, TCHAR** tokens, int count)
	{
		// return tokens count
		if (nullptr == tokens)
		{
			count = 0;
			TCHAR* pc = _tcschr(szString, delimiter);
			count++;
			while (pc != NULL)
			{
				pc = _tcschr(pc + 1, delimiter);
				count++;
			}
			return count;
		}
		// 0 for maximum
		TCHAR szDelimiters[2] = { delimiter, 0 };
		TCHAR* ptr = _tcstok(szString, szDelimiters);
		bool hasCount = 0 != count;
		int leftCount = count;
		while (ptr != NULL && (hasCount ? leftCount : true))
		{
			*tokens++ = ptr;
			ptr = _tcstok(NULL, szDelimiters);
			leftCount--;
		}
		return count - leftCount;
	}

	static COLORREF LightColor(COLORREF color, double percent)
	{
		int r = (int)(GetRValue(color) + (0xFF - GetRValue(color)) * percent);
		int g = (int)(GetGValue(color) + (0xFF - GetGValue(color)) * percent);
		int b = (int)(GetBValue(color) + (0xFF - GetBValue(color)) * percent);
		return RGB(r, g, b);
	}
};

