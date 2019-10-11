#pragma once
#include "Mass.h"
#include <list>
#include <vector>
using namespace std;

class GameFrame;
class TetrisShape;

struct MassUnit
{
	bool isSolid = false;
	char color = 0; // -1 for mass color, others for tetris colors
};

typedef vector<MassUnit> MassLine;
typedef list<MassLine*> MassBlock;

class Mass
{
public:
	bool HasTouched(TetrisShape* pTetrisShape);
	int DistanceFrom(TetrisShape* pTetrisShape);
	bool CanInsert(TetrisShape* pTetrisShape, int x, int y);
	int FindBottommostBlankY(TetrisShape* pTetrisShape, int x);
	bool HasBlank(TetrisShape* pTetrisShape, int x);

	void Union(TetrisShape* pTetrisShape);
	void RemoveLine(int line);
	void GenerateLine(int line, int blankRate);

	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);
	int GetTop();
	int GetBottom();
	int GetRight();
	int GetHeight();
	bool IsSolid(int x, int y);
	bool IsSolid(MassLine *pMassLine, int x);
	int GetTopmostSolidY(int x);

	void SetGameFrame(GameFrame* pGameFrame);
	MassUnit* GetUnit(int x, int y);
	MassLine* GetLine(int line);
	MassBlock::iterator GetLineIterator(int line);
	MassBlock::iterator GetFirstLineIterator();
	MassBlock::iterator GetLastLineIterator();

private:
	MassLine* CreateLine();
	void InsertLine(int at, MassLine *pMassLine);
	void InsertLines(int at, MassBlock* pMassBlock);
	void DeleteLine(int at);
	void DeleteLines(int at, int count);

	MassBlock massBlock;
	int top;
	
	GameFrame* pGameFrame;
};
