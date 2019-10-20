#include "Controller.h"
#include "Configuration.h"
#include "GameFrame.h"
#include "Archive.h"

Controller Controller::singleton;

void Controller::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

void Controller::Initialize(Configuration* pConfiguration)
{
	this->stepDownTimespan = pConfiguration->stepDownTimespan;
	this->dropTimespan = pConfiguration->dropTimespan;
	this->dropDelayTimespan = pConfiguration->dropDelay;
	this->removeDelayTimespan = pConfiguration->removeDelay;
	this->removeBlinkTimespan = pConfiguration->removeBlinkTimespan;
}

void Controller::SetHWnd(HWND hWnd)
{
	this->hWnd = hWnd;
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

void Controller::End()
{
	gameState = GameState::End;
	pGameFrame->End();
}

void Controller::Pause()
{
	gameState = GameState::Pause;
}

void Controller::Resume()
{
	gameState = GameState::Start;
}

bool Controller::IsStarted()
{
	return gameState!=GameState::End;
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
