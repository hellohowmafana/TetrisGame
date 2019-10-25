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
	this->removeBlinkTimespan = pConfiguration->removeBlinkTimespan;
	this->removeBlinkTimes = pConfiguration->removeBlinkTimes;
	removeBlinkCount = 0;

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

void Controller::SetDrawer(Drawer* pDrawer)
{
	this->pDrawer = pDrawer;
}

bool Controller::IsInitialized()
{
	return initialized;
}

GameState Controller::GetGameState()
{
	return gameState;
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
		EndStepDown();
		StartDropDelay();
	}
}

void Controller::Drop()
{
	if (GameState::Start != gameState)
		return;
	pGameFrame->Drop();
	EndStepDown();
	StartDropDelay();
}

void Controller::Start()
{
	gameState = GameState::Start;
	StartStepDown(false);
}

void Controller::End()
{
	gameState = GameState::End;
	EndStepDown();
	EndDropDelay();
	EndRemoveBlink();
}

void Controller::Pause()
{
	EndStepDown();
	EndDropDelay();
	EndRemoveBlink();
	gameState = GameState::Pause;
}

void Controller::Resume()
{
	gameState = GameState::Start;
	StartStepDown(false);
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

bool Controller::StartStepDown(bool isDropping)
{
	return SetTimer(hWnd, ST_STEPDOWN,
		isDropping ? dropTimespan : stepDownTimespan,
		StepDownTimerProcStatic);
}

bool Controller::EndStepDown()
{
	return KillTimer(hWnd, ST_STEPDOWN);
}

void Controller::StepDownTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.StepDownTimerProc(hWnd, msg, id, millisecond);
}

void Controller::StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	StepDown();
	InvalidDraw();
}

bool Controller::StartDropDelay()
{
	return SetTimer(hWnd, ST_DROPDELAY, dropDelayTimespan, DropDelayTimerProcStatic);
}

bool Controller::EndDropDelay()
{
	return KillTimer(hWnd, ST_DROPDELAY);
}

void Controller::DropDelayTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.DropDelayTimerProc(hWnd, msg, id, millisecond);
}

void Controller::DropDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	EndDropDelay();

	if (pGameFrame->Union() > 0)
	{
		InvalidDraw();
		StartRemoveBlink();
		return;
	}
	if (pGameFrame->IsFull())
	{
		End();
		return;
	}
	pGameFrame->RebornTetrisShape();
	InvalidDraw();
	StartStepDown(false);
}

bool Controller::StartRemoveBlink()
{
	removeBlinkCount = 0;
	return SetTimer(hWnd, ST_REMOVEBLINK, removeBlinkTimespan, RemoveBlinkTimerProcStatic);
}

bool Controller::EndRemoveBlink()
{
	removeBlinkCount = 0;
	return KillTimer(hWnd, ST_REMOVEBLINK);
}

void Controller::RemoveBlinkTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.RemoveBlinkTimerProc(hWnd, msg, id, millisecond);
}

void Controller::RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	if (removeBlinkTimes == removeBlinkCount)
	{
		EndRemoveBlink();
		pGameFrame->SetBlinkState(BlinkState::None);

		pGameFrame->RemoveFullLines();
		pGameFrame->RebornTetrisShape();
		
		InvalidDraw();

		StartStepDown(false);
	}
	else
	{
		removeBlinkCount++;

		BlinkState blinkState = pGameFrame->GetBlinkState();
		pGameFrame->SetBlinkState(
			BlinkState::None == blinkState || BlinkState::Normal == blinkState ?
			BlinkState::Light : BlinkState::Normal);
		
		InvalidDraw();
	}
}

void Controller::InvalidDraw()
{
	pDrawer->Invalid();
}
