#pragma once
#include "TetrisType.h"
#include <vector>
#include <tchar.h>
using namespace std;

enum class TetrisRotation {TetrisRotation0, TetrisRotation1, TetrisRotation2,	TetrisRotation3};

class TetrisShape
{
public:
	TetrisShape(TetrisType* pTetrisType);

public:
	void Rotate();
	void Move();
	void StepDown();
	void DropDown();

	bool ValidateXY(int x, int y);
	char GetData(int x, int y);
	bool IsSolid(int x, int y);
	int GetWidth();
	int GetHeight();
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();
	int GetColor();

private:
	TetrisType* pTetrisType;
	int posX, posY;
	TetrisRotation rotation;
};

