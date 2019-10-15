#pragma once
#include "GameFrame.h"

enum class GameState
{
	Start, Stop, Pause, Resume
};

class Controller
{
public:
	static Controller singleton;
	void SetGameFrame(GameFrame* pGameFrame);

	void Rotate();
	void StepHorizontal(bool left);
	void StepDown();
	void Drop();

	void Start();
	void Stop();
	void Pause();
	void Resume();

	bool IsStarted();

public:
	GameFrame* GetGameFrame();
	TetrisShape* GetTetrisShape();
	TetrisShape* GetNextTetrisShape();
	Mass* GetMass();

private:
	Controller() {};
	GameFrame* pGameFrame;
	GameState gameState;
};

