#pragma once
#include <list>
#include <map>
#include <string>
using namespace std;

class TetrisType;
typedef list<TetrisType*> TetrisTypeGroup;
typedef map<wstring, TetrisTypeGroup*> TetrisTypeLibrary;

class TetrisType
{
public:
	static void Create(wstring group, wstring name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	static void Delete(TetrisType* pTetrisType);
	static void Clear();
	static TetrisTypeGroup* CreateGroup(wstring group);
	static void DeleteGroup(wstring group);
	static bool ExsitGroup(wstring group);
	static TetrisType* GetTetrisType(wstring group, wstring name);
	static TetrisTypeGroup* GetGroup(wstring group);
	static int GetTetrisTypesCount();
	static int GetTetrisTypesCount(wstring group);
	static int GetRandomColor(wstring group);
	static TetrisType* Random();

private:
	TetrisType(wstring group, wstring name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	~TetrisType();
	bool Initialize(wstring group, wstring name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);

	int GetPostion(int x, int y);
	void GetXY(int pos, int* px, int* py);

public:
	static const wstring classic;
	wstring group;
	wstring name;
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
