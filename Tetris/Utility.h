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
	};
};

