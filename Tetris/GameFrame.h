#pragma once
#include "Configuration.h"
#include "Mass.h"
#include "TetrisShape.h"
#include "UnitFrame.h"
#include "ISerializable.h"
#include <Windows.h>

class PromptFrame;
class InfoFrame;

enum class GameState
{
	Start, Pause, End
};

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
	GameState gameState;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;

private:
	GameFrame() {};

public:
	void Initialize(Configuration* pConfiguration);
	void SetPromptFrame(PromptFrame* pPromptFrame);
	void SetInfoFrame(InfoFrame* pInfoFrame);
	void InitializeGame();

	void Start();
	void End();
	void Pause();
	void Resume();
	GameState GetGameState();

	void StepLeft();
	void StepRight();
	void StepDown();
	void Drop();
	void EndDrop();
	void Rotate();

	Mass* GetMass();
	TetrisShape* GetShape();
	TetrisShape* GetNextShape();

public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString);
	virtual bool Load(const TCHAR* szSection, TCHAR* szString);
};

