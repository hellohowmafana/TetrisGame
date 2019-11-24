#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "UnitFrame.h"
#include "ISerializable.h"
#include <Windows.h>

class PromptFrame;
class InfoFrame;

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
	wstring pathUnitBitmap;
	wstring pathPauseIcon;
	wstring pathResumeIcon;
	wstring pathBeginSplash;
	wstring pathGameOverSplash;

	double iconScaleRatio;
	double maskTransparency;

public:
	vector<MassLine*> vecLastFullLines;
	int rolledRows;

	int score;
	int level;

private:
	Mass mass;
	TetrisShape tetrisShape;
	TetrisShape nextTetrisShape;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;

private:
	GameFrame();

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
	bool HasFullLine();
	void RemoveFullLines();
	void Rotate();
	void RebornTetrisShape();
	bool IsFull();

	bool IsLastFullLine(const MassLine* pMassLine);

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(const wstring label, wstring& value);
	virtual bool Load(const wstring label, wstring value);
};

