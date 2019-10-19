#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "UnitFrame.h"
#include "ISerializable.h"
#include <Windows.h>

class GameFrame : public UnitFrame, public ISerializable
{
public:
	static GameFrame singleton;

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

	void StepLeft();
	void StepRight();
	void StepDown();
	void Drop();
	void Rotate();

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString);
	virtual bool Load(const TCHAR* szSection, TCHAR* szString);
};

