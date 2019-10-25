#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "UnitFrame.h"
#include "ISerializable.h"
#include <Windows.h>

class PromptFrame;
class InfoFrame;

enum class BlinkState { None, Light, Normal };

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

private:
	Mass mass;
	TetrisShape tetrisShape;
	TetrisShape nextTetrisShape;
	int score;
	int level;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;

	BlinkState blinkState;
	vector<MassLine*> vecLastFullLines;

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

	void SetBlinkState(BlinkState blinkState);
	BlinkState GetBlinkState();
	bool IsBlinkingLight();
	bool IsBlinkingLine(MassLine* pMassLine);

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString);
	virtual bool Load(const TCHAR* szSection, TCHAR* szString);
};

