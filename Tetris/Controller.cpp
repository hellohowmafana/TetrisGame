#include "Controller.h"
#include "Configuration.h"
#include "GameFrame.h"
#include "Level.h"
#include "Archive.h"

Controller Controller::singleton;

void Controller::Initialize(Configuration* pConfiguration)
{
	this->stepDownTimespan = Level::GetLevel(pConfiguration->startLevel)->stepDownTimeSpan;
	this->dropTimespan = pConfiguration->dropTimespan;
	this->dropDelayTimespan = pConfiguration->dropDelay;
	this->removeDelayTimespan = pConfiguration->removeDelay;
	this->removeBlinkTimespan = pConfiguration->removeBlinkTimespan;

	initialized = true;
	gameState = GameState::End;
}

void Controller::SetHWnd(HWND hWnd)
{
	this->hWnd = hWnd;
}

void Controller::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

bool Controller::IsInitialized()
{
	return initialized;
}

void Controller::Rotate()
{
	if (GameState::Start != gameState)
		return;
	pGameFrame->Rotate();
}

void Controller::StepHorizontal(bool left)
{
	if (GameState::Start != gameState)
		return;
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
	if (GameState::Start != gameState)
		return;
	if (pGameFrame->StepDown())
	{
		pGameFrame->Union();
		pGameFrame->RemoveFullLines();
		if (pGameFrame->IsFull())
		{
			End();
			return;
		}
		pGameFrame->RebornTetrisShape();
	}
}

void Controller::Drop()
{
	if (GameState::Start != gameState)
		return;
	pGameFrame->Drop();
	pGameFrame->Union();
	pGameFrame->RemoveFullLines();
	if (pGameFrame->IsFull())
	{
		End();
		return;
	}
	pGameFrame->RebornTetrisShape();
}

void Controller::Start()
{
	gameState = GameState::Start;
	//SetStepDownTimer();
}

void Controller::End()
{
	gameState = GameState::End;
	MessageBox(0, L"", L"", 0);
}

void Controller::Pause()
{
	gameState = GameState::Pause;
}

void Controller::Resume()
{
	gameState = GameState::Start;
}

void Controller::Restart()
{
	End();
	Start();
}

bool Controller::SaveGame(TCHAR* szArchive)
{
	return false;
}

bool Controller::LoadGame(TCHAR* szArchive)
{
	if (0 == _tcslen(szArchive))
		return false;
	End();
	Archive::Load(szArchive, this);
	Start();
	return true;
}

GameFrame* Controller::GetGameFrame()
{
	return pGameFrame;
}

TetrisShape* Controller::GetTetrisShape()
{
	return pGameFrame->GetShape();
}

TetrisShape* Controller::GetNextTetrisShape()
{
	return pGameFrame->GetNextShape();
}

Mass* Controller::GetMass()
{
	return pGameFrame->GetMass();
}

void Controller::SetStepDownTimer()
{
	SetTimer(hWnd, ST_STEPDOWN, stepDownTimespan, StepDownTimerProc);
}

void Controller::KillStepDownTimer()
{
	KillTimer(hWnd, ST_STEPDOWN);
}

void Controller::StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller* pController = &Controller::singleton;
	pController->StepDown();
}

void Controller::SetDropTimer()
{
	SetTimer(hWnd, ST_DROP, dropTimespan, DropTimerProc);
}

void Controller::KillDropTimer()
{
	KillTimer(hWnd, ST_DROP);
}

void Controller::DropTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller* pController = &Controller::singleton;
	pController->Drop();
}

void Controller::SetDropDelayTimer()
{
	SetTimer(hWnd, ST_DROPDELAY, dropDelayTimespan, DropDelayTimerProc);
}

void Controller::KillDropDelayTimer()
{
	KillTimer(hWnd, ST_DROPDELAY);
}

void Controller::DropDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
}

void Controller::SetRemoveDelayTimer()
{
	SetTimer(hWnd, ST_REMOVEDELAY, removeDelayTimespan, RemoveDelayTimerProc);
}

void Controller::KillRemoveDelayTimer()
{
	KillTimer(hWnd, ST_REMOVEDELAY);
}

void Controller::RemoveDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
}

void Controller::SetRemoveBlinkTimer()
{
	SetTimer(hWnd, ST_REMOVEBLINK, removeBlinkTimespan, RemoveBlinkTimerProc);
}

void Controller::KillRemoveBlinkTimer()
{
	KillTimer(hWnd, ST_REMOVEBLINK);
}

void Controller::RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
}
