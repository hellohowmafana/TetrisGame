#pragma once
#include <list>
#include <vector>
#include "ISerializable.h"
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

class Mass : public ISerializable
{
public:
	bool Initialize();

	bool IsEmpty();
	int GetTopmostSolidY(int x, int yFrom);
	bool IsTouched(TetrisShape* pTetrisShape);
	int CalculateDistanceY(TetrisShape* pTetrisShape);

	int FindBottommostBlankY(TetrisShape* pTetrisShape, int x);
	bool HasBlank(TetrisShape* pTetrisShape, int x);

	bool HitTest(TetrisShape* pTetrisShape);
	bool Union(TetrisShape* pTetrisShape);
	bool IsLineFull(int line);
	bool IsLineFull(MassLine* pMassLine);
	void RemoveLine(int line);
	int RemoveFullLines(int from, int to);
	bool GenerateLines(int line, double blankRate);
	bool GenerateLines(int line, int count, double blankRate);
	void Clear();
	bool IsFull();
	vector<MassLine*>* GetFullLines(vector<MassLine*>* pvecLines, int from, int to);

	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);
	int GetTop();
	int GetBottom();
	int GetRight();
	int GetHeight();
	bool IsSolid(int x, int y);
	bool IsSolid(MassLine *pMassLine, int x);

	void SetGameFrame(GameFrame* pGameFrame);
	GameFrame* GetGameFrame();
	MassUnit* GetUnit(int x, int y);
	MassLine* GetLine(int line);
	MassBlock::iterator GetLineIterator(int line);
	MassBlock::iterator GetTopLineIterator();
	MassBlock::iterator GetBottomLineIterator();

private:
	MassLine* CreateLine();
	void InsertLine(int at, MassLine *pMassLine);
	void InsertLines(int at, MassBlock* pMassBlock);
	void DeleteLine(int at);
	void DeleteLine(MassLine* pMassLine);
	void DeleteLines(int at, int count);
	void ClearLines();

	MassBlock massBlock;
	int top;
	
	GameFrame* pGameFrame;

public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString);
	virtual bool Load(const TCHAR* szSection, TCHAR* szString);
};
