#pragma once
#include "TetrisType.hpp"
#include <vector>
#include "ISerializable.hpp"
#include "IBinarySerializable.hpp"
using namespace std;

class UnitFrame;

enum class TetrisRotation {Rotation0, Rotation1, Rotation2, Rotation3};

class TetrisShape : public ISerializable, public IBinarySerializable
{
public:
	TetrisShape();
	void InitializeRandom();
	void Initialize(TetrisType* pTetrisType, TetrisRotation rotation, int randomColor);
	void RebornRandom();
	void Reborn(TetrisType* pTetrisType, TetrisRotation rotation, int randomColor);
	void SetUseRandomColor(bool useRandomColor);
	TetrisShape* Clone(TetrisShape* pTetrisShape);
	TetrisType* GetType();
	TetrisRotation GetRotation();

	void CenterHorizontal(bool leanLeft, bool careOffset);
	void SetTopCenterPostion(bool leanLeft, bool careOffset);
	void CenterPostion(bool leanLeft, bool leanTop);
	bool SetPostion(int x, int y);
	bool Move(int offestX, int offsetY);
	bool MoveTo(int x, int y);
	void CalculateRotationPosition(TetrisRotation dst, TetrisRotation src, int* pPosX, int* pPosY);
	bool CanRotate();
	bool Rotate();
	bool RotateBack();
	bool StepLeft();
	bool StepRight();
	bool StepDown();
	bool StepUp();
	bool IsPenerable();

	bool TestX(int x, bool frameCoordinate);
	bool TestY(int y, bool frameCoordinate);
	bool TestXY(int x, int y, bool frameCoordinate);
	bool TestShape(int posX, int posY, bool frameCoordinate);
	char GetData(int x, int y, bool frameCoordinate);
	bool IsSolid(int x, int y, bool frameCoordinate);
	int GetWidth();
	int GetHeight();
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();
	int GetColor();
	int GetRandomColor();
	
	bool IsOnTop();
	bool IsOnBottom();
	int GetBottommostSolidY(int x, bool frameCoordinate);

	void SetCurrent(bool current);
	void SetFrame(UnitFrame* pUnitFrame);
	UnitFrame* GetFrame();

private:
	TetrisType* pTetrisType;
	int posX, posY;
	TetrisRotation rotation;
	bool useRandomColor;
	int randomColor;
	bool current;
	UnitFrame* pUnitFrame;

public:
	virtual bool Save(const wstring label, wstring& value);
	virtual bool Load(const wstring label, wstring value);

	virtual bool Save(char* pData, unsigned int& size, char argument);
	virtual bool Load(char* pData);

	TetrisRotation IntToTetrisRotation(int irotation);
	int TetrisRotationToInt(TetrisRotation rotation);
};

