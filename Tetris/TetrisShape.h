#pragma once
#include "TetrisType.h"
#include <vector>
#include <tchar.h>
using namespace std;

class GameFrame;
enum class TetrisRotation {Rotation0, Rotation1, Rotation2, Rotation3};

class TetrisShape
{
public:
	TetrisShape();
	void InitializeRandom();
	void Initialize(TetrisType* pTetrisType, TetrisRotation rotation);

public:
	bool Move(int offestX, int offsetY);
	bool Rotate();
	bool StepLeft();
	bool StepRight();
	bool StepDown();

	bool ValidateX(int x, bool frameCoordinate);
	bool ValidateY(int y, bool frameCoordinate);
	bool ValidateXY(int x, int y, bool frameCoordinate);
	char GetData(int x, int y, bool frameCoordinate);
	bool IsSolid(int x, int y, bool frameCoordinate);
	int GetWidth();
	int GetHeight();
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();
	int GetColor();
	
	int GetBottommostSolidY(int x, bool frameCoordinate = false);

	void SetGameFrame(GameFrame* pGameFrame);

private:
	TetrisType* pTetrisType;
	int posX, posY;
	TetrisRotation rotation;

	GameFrame* pGameFrame;
};

