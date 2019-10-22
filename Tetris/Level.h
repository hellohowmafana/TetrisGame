#pragma once
#include <vector>
using namespace std;
class Level
{
public:
	int level;
	int minScore; // include, -1 for infinate
	int maxScore; // exclude, -1 for infinate
	int stepDownTimeSpan;
	double scoreRate;

	static vector<Level> vecLevels;
	static Level* GetLevel(int level);
	static Level* JudgeLevel(int score);
};

