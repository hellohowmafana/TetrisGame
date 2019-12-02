#include "Level.hpp"

vector<Level> Level::vecLevels;

Level* Level::GetLevel(int level)
{
	if (level < 1 || level > (int)vecLevels.size())
		return nullptr;
	return &vecLevels[level - 1];
}

Level* Level::JudgeLevel(int score)
{
	if (score < 0)
		return nullptr;
	for (size_t i = 0; i < vecLevels.size(); i++)
	{
		if ((vecLevels[i].minScore == -1 ? false : score >= vecLevels[i].minScore) &&
			(vecLevels[i].maxScore == -1 ? true : score < vecLevels[i].maxScore))
			return &vecLevels[i];
	}
	return nullptr;
}
