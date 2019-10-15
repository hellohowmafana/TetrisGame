#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "ISerializable.h"
#include <Windows.h>

class GameFrame : ISerializable
{
public:
	static GameFrame singleton;

	int left;
	int top;
	int sizeX;
	int sizeY;
	int unitWidth;
	int borderThickness;
	int separatorThickness;
	double startLineBlankRate;
	bool useColor;
	bool useColorRandom;
	bool useMassColor;

	COLORREF* pBorderColor;
	COLORREF* pSeparatorColor;
	vector<COLORREF>* pTetrisColors;
	COLORREF* pMassColor;

private:
	Mass mass;
	TetrisShape tetrisShape;
	TetrisShape nextTetrisShape;
	int score;

private:
	GameFrame() {};

public:
	void Initialize(Configuration* pConfiguration);
	void Start();
	void Stop();
	void Pause();
	void Resume();

	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);
	void Insert(Mass* pMass, TetrisShape* pTetrisShape);
	bool HasTouched(Mass* pMass, TetrisShape* pTetrisShape);
	void StepLeft();
	void StepRight();
	void StepDown();
	void Drop();
	void Rotate();

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(TCHAR* szString);
	virtual bool Load(TCHAR* szString);
};

