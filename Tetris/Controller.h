#pragma once
#include "TetrisShape.h"

class GameFrame;

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

private:
	Controller() {};
	GameFrame* pGameFrame;
	GameState gameState;
};

