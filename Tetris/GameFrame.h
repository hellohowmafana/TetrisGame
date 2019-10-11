#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include <Windows.h>

class GameFrame
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

	COLORREF* pBorderColor;
	COLORREF* pSeparatorColor;
	vector<COLORREF>* pTetrisColors;
	COLORREF* pMassColor;

private:
	Mass mass;
	TetrisShape tetrisShape;

private:
	GameFrame() {};

public:
	void Initialize(Configuration* pConfiguration);
	void Start();
	void Stop();
	void Pause();
	void Resume();

	void Insert(Mass* pMass, TetrisShape* pTetrisShape);
	bool HasTouched(Mass* pMass, TetrisShape* pTetrisShape);
	void StepLeft();
	void StepRight();
	void StepDown();
	void Drop();
	void Rotate();

	Mass* GetMass();
	TetrisShape* GetShape();
};

