#pragma once
#include <random>
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
};

