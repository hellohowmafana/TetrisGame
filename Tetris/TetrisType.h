#pragma once
#include <tchar.h>
#include <list>
using namespace std;

class TetrisType
{
public:
	static void Create(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color);
	static void Delete(TetrisType* pTetrisType);
	static void Clear(TetrisType* pTetirsType);

private:
	TetrisType(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color);
	~TetrisType();
	bool Initialize(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color);

	int GetPostion(int x, int y);
	void GetXY(int pos, int* px, int* py);

public:
	TCHAR* group;
	TCHAR* name;
	bool penetrable;
	int row;
	int col;
	char* pData;
	int color;
	int centerX, centerY; // for rotate center
	bool canRotate;

private:
	static list<TetrisType*> lstTetrisTypes;
};

