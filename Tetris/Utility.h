#pragma once
#include <random>
#include <tchar.h>
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
		TCHAR szDelimiters[2] = { delimiter, 0 };
		TCHAR* ptr = _tcstok(szString, szDelimiters);
		int leftCount = count;
		while (ptr != NULL && leftCount)
		{
			*tokens++ = ptr;
			ptr = _tcstok(NULL, szDelimiters);
			leftCount--;
		}
		return count - leftCount;
	}
};

