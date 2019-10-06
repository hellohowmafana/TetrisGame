#pragma once
#include <tchar.h>
#include <vector>
using namespace std;

#define TETRIS_GROUP_CHARS 16
#define TETRIS_TYPE_CHARS 16

class TetrisType
{
public:
	TetrisType(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, size_t color);
	bool Initialize(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, size_t color);
	void Append(TetrisType* pTetrisType);
	void Remove(TetrisType* pTetirsType);

protected:
	TCHAR* group;
	TCHAR* name;
	bool penetrable;
	int row;
	int col;
	char* pData;
	size_t color;

private:
	vector<TetrisType> vecTetrisTypes;
};

