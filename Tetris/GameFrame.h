#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "UnitFrame.h"
#include "ISerializable.h"
#include <Windows.h>

class PromptFrame;
class InfoFrame;

enum class GameFrameState { None, Normal, BlinkLight, BlinkNormal, RollUp, RollDown };

class GameFrame : public UnitFrame, public ISerializable
{
public:
	static GameFrame singleton;

	int startLevel;
	int startLine;
	double startLineBlankRate;
	vector<int> vecRemoveScores;
	int droppedScore;
	bool useColor;
	bool useColorRandom;
	bool useMassColor;

	vector<COLORREF>* pTetrisColors;
	COLORREF* pMassColor;

public:
	GameFrameState state;
	vector<MassLine*> vecLastFullLines;
	int rolledRows;

private:
	Mass mass;
	TetrisShape tetrisShape;
	TetrisShape nextTetrisShape;
	int score;
	int level;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;

private:
	GameFrame() {};

public:
	void Initialize(Configuration* pConfiguration);
	void SetPromptFrame(PromptFrame* pPromptFrame);
	void SetInfoFrame(InfoFrame* pInfoFrame);
	void InitializeGame();
	void Reborn();

	void StepLeft();
	void StepRight();
	bool StepDown();
	void Drop();
	int Union();
	void RemoveFullLines();
	void Rotate();
	void RebornTetrisShape();
	bool IsFull();

	bool IsBlinkingLight();
	bool IsBlinkingLine(MassLine* pMassLine);

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString);
	virtual bool Load(const TCHAR* szSection, TCHAR* szString);
};

