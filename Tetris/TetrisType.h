#pragma once
#include <tchar.h>
#include <list>
using namespace std;

class TetrisType;
typedef list<TetrisType*> TetrisTypeGroup;
typedef list<TetrisTypeGroup*> TetrisTypeLibrary;

class TetrisType
{
public:
	static void Create(TCHAR const* group, TCHAR const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	static void Delete(TetrisType* pTetrisType);
	static void Clear();
	static TetrisTypeGroup* CreateGroup();
	static void DeleteGroup(TCHAR const* group);
	static bool ExsitGroup(TCHAR const* group);
	static TetrisType* GetTetrisType(TCHAR const* group, TCHAR const* name);
	static TetrisTypeGroup* GetGroup(TCHAR const* group);
	static TetrisType* Random();

private:
	TetrisType(TCHAR const* group, TCHAR const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	~TetrisType();
	bool Initialize(TCHAR const* group, TCHAR const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);

	int GetPostion(int x, int y);
	void GetXY(int pos, int* px, int* py);

public:
	TCHAR* group;
	TCHAR* name;
	bool penetrable;
	bool twoRotation;
	bool clockwiseRotation;
	int horizontalCenterOffset;
	int row;
	int col;
	char* pData;
	int color;
	int centerX, centerY; // for rotate center
	bool canRotate;

private:
	static TetrisTypeLibrary tetrisTypeLibrary;
};
