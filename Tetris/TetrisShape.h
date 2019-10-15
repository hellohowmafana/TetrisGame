#pragma once
#include "TetrisType.h"
#include <vector>
#include <tchar.h>
#include "ISerializable.h"
using namespace std;

class GameFrame;
enum class TetrisRotation {Rotation0, Rotation1, Rotation2, Rotation3};

class TetrisShape : ISerializable
{
public:
	TetrisShape();
	void InitializeRandom();
	void Initialize(TetrisType* pTetrisType, TetrisRotation rotation);
	void Reborn();

public:
	void CenterHorizontal(bool leanLeft);
	void SetTopCenterPostion(bool leanLeft);
	bool SetPostion(int x, int y);
	bool Move(int offestX, int offsetY);
	void CalculateRotationPosition(TetrisRotation dst, TetrisRotation src, int* pPosX, int* pPosY);
	bool Rotate();
	bool StepLeft();
	bool StepRight();
	bool StepDown();
	bool StepUp();

	bool ValidateX(int x, bool frameCoordinate);
	bool ValidateY(int y, bool frameCoordinate);
	bool ValidateXY(int x, int y, bool frameCoordinate);
	bool ValidateShape(int posX, int posY, bool frameCoordinate);
	char GetData(int x, int y, bool frameCoordinate);
	bool IsSolid(int x, int y, bool frameCoordinate);
	int GetWidth();
	int GetHeight();
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();
	int GetColor();
	
	int GetBottommostSolidY(int x, bool frameCoordinate);

	void SetGameFrame(GameFrame* pGameFrame);
	GameFrame* GetGameFrame();

private:
	TetrisType* pTetrisType;
	int posX, posY;
	TetrisRotation rotation;

	GameFrame* pGameFrame;

public:
	virtual bool Save(TCHAR* szString);
	virtual bool Load(TCHAR* szString);
	TetrisRotation IntToTetrisRotation(int irotation);
};

