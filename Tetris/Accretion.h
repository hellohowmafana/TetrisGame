#pragma once
#include "TetrisShape.h"
#include <list>
#include <vector>
using namespace std;

struct AccretionUnit
{
	bool isSolid = false;
	char color = 0; // -1 for accretion color, others for tetris colors
};

typedef vector<AccretionUnit> AccretionLine;
typedef list<AccretionLine*> AccretionBlock;

class Accretion
{
private:
	Accretion();

public:
	void Insert(TetrisShape* pTetrisShape);
	void RemoveLine(int line);
	void GenerateLine(int line, int blankRate);

	int GetTop();
	bool IsSolid(int x, int y);

private:
	AccretionLine* CreateLine();
	void InsertLine(int at, AccretionLine *pAccretionLine);
	void InsertLines(int at, AccretionBlock* pAccretionBlock);
	void DeleteLine(int at);
	void DeleteLines(int at, int count);

	AccretionUnit* GetUnit(int x, int y);
	AccretionLine* GetLine(int line);

	AccretionBlock accretionBlock;
	int top;
};
