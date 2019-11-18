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
	static void Create(wchar_t const* group, wchar_t const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	static void Delete(TetrisType* pTetrisType);
	static void Clear();
	static TetrisTypeGroup* CreateGroup(wchar_t const* group);
	static void DeleteGroup(wchar_t const* group);
	static bool ExsitGroup(wchar_t const* group);
	static TetrisType* GetTetrisType(wchar_t const* group, wchar_t const* name);
	static TetrisTypeGroup* GetGroup(wchar_t const* group);
	static int GetTetrisTypesCount();
	static int GetTetrisTypesCount(wchar_t const* group);
	static int GetRandomColor(wchar_t const* group);
	static TetrisType* Random();

private:
	TetrisType(wchar_t const* group, wchar_t const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);
	~TetrisType();
	bool Initialize(wchar_t const* group, wchar_t const* name,
		bool penetrable, bool twoRotation, bool clockwiseRotation,
		int horizontalCenterOffset,
		int row, int col, char* pData, size_t dataCount, int color);

	int GetPostion(int x, int y);
	void GetXY(int pos, int* px, int* py);

public:
	static const wchar_t* classic;
	wchar_t* group;
	wchar_t* name;
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
