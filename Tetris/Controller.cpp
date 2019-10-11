#include "Controller.h"
#include "GameFrame.h"

Controller Controller::singleton;

void Controller::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

void Controller::Rotate()
{
	pGameFrame->Rotate();
}

void Controller::StepHorizontal(bool left)
{
	if (left)
	{
		pGameFrame->StepLeft();
	}
	else
	{
		pGameFrame->StepRight();
	}
}

void Controller::StepDown()
{
	pGameFrame->StepDown();
}

void Controller::Drop()
{
	pGameFrame->Drop();
}

void Controller::Start()
{
	gameState = GameState::Start;
	pGameFrame->Start();
}

void Controller::Stop()
{
	gameState = GameState::Stop;
	pGameFrame->Stop();
}

void Controller::Pause()
{
	gameState = GameState::Pause;
}

void Controller::Resume()
{
	gameState = GameState::Resume;
}

bool Controller::IsStarted()
{
	return gameState!=GameState::Stop;
}
