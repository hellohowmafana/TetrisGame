#include "Controller.hpp"
#include "Configuration.hpp"
#include "Level.hpp"
#include "Archive.hpp"
#include "Drawer.hpp"
#include "Musician.hpp"

Controller Controller::singleton;

void Controller::Initialize(Configuration* pConfiguration)
{
	this->dropImmediate = pConfiguration->dropImmediate;
	this->stepDownTimespan = Level::GetLevel(pConfiguration->startLevel)->stepDownTimeSpan;
	this->dropTimespan = pConfiguration->dropTimespan;
	this->stepHorizontalTimespan = pConfiguration->stepHorizontalTimespan;
	this->rotateTimespan = pConfiguration->rotateTimespan;
	this->removeBlinkTimespan = pConfiguration->removeBlinkTimespan;
	this->removeBlinkCount = pConfiguration->removeBlinkCount;
	removeBlinkTimes = 0;
	this->rollTimespan = pConfiguration->rollTimespan;
	this->resumeDelayTimespan = pConfiguration->resumeDelayTimespan;

	UpdateInputSettings(pConfiguration);

	soundOn = pConfiguration->soundOn;
	bgmOn = pConfiguration->bgmOn;

	initialized = true;
	gameState = GameState::None;
}

void Controller::UpdateInputSettings(Configuration* pConfiguration)
{
	this->actionLeft = pConfiguration->actionLeft;
	this->actionRight = pConfiguration->actionRight;
	this->actionDown = pConfiguration->actionDown;
	this->actionRotate = pConfiguration->actionRotate;
	this->actionDrop = pConfiguration->actionDrop;
	this->operationPause = pConfiguration->operationPause;
	this->operationRestart = pConfiguration->operationRestart;
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

Drawer* Controller::GetDrawer()
{
	return pDrawer;
}

void Controller::SetMusician(Musician* pMusician)
{
	this->pMusician = pMusician;
}

Musician* Controller::GetMusician()
{
	return pMusician;
}

bool Controller::IsInitialized()
{
	return initialized;
}

bool Controller::IsResourceInitialized()
{
	return pDrawer->IsInitialized() && pMusician->IsInitialized();
}

GameState Controller::GetGameState()
{
	return gameState;
}

bool Controller::IsStarted()
{
	return GameState::None != gameState &&
		GameState::End != gameState &&
		GameState::RollUp != gameState &&
		GameState::RollDown != gameState;
}

bool Controller::IsStarting()
{
	return GameState::None != gameState &&
		GameState::End != gameState &&
		GameState::RollUp != gameState &&
		GameState::RollDown != gameState &&
		GameState::Pause != gameState;
}

bool Controller::IsBlinking()
{
	return GameState::BlinkLight == gameState ||
		GameState::BlinkNormal == gameState;
}

bool Controller::IsAvailable()
{
	return IsStarting() && !IsBlinking();
}

void Controller::OnKeyDown(WPARAM keyCode)
{
	OnInput(keyCode, true);
}

void Controller::OnKeyUp(WPARAM keyCode)
{
	OnInput(keyCode, false);
}

void Controller::OnMouseDown(WPARAM buttonCode)
{
	OnInput(buttonCode, true);
}

void Controller::OnMouseUp(WPARAM buttonCode)
{
	OnInput(buttonCode, false);
}

void Controller::OnInput(WPARAM inputCode, bool isKeyDown)
{
	if (isKeyDown)
	{
		if (inputCode == actionLeft)
		{
			if (IsStarting())
			{
				if (!startingStepLeft)
				{
					StepHorizontal(true);
					StartStepHorizontal(true);
				}
			}
		}
		else if (inputCode == actionRight)
		{
			if (IsStarting())
			{
				if (!startingStepRight)
				{
					StepHorizontal(false);
					StartStepHorizontal(false);
				}
			}
		}
		else if (inputCode == actionRotate)
		{
			if (IsStarting())
			{
				if (!startingRotate)
				{
					Rotate();
					StartRotate();
				}
			}
		}
		else if (inputCode == actionDown)
		{
			if (IsStarting())
			{
				if (!startingDrop)
				{
					StepDown();
					StartStepDown(true);
				}
			}
		}
		else if (inputCode == actionDrop)
		{
			if (IsStarting())
			{
				if (dropImmediate)
				{
					Drop();
				}
				else
				{
					if (!startingDrop)
					{
						StepDown();
						StartStepDown(true);
					}
				}
			}
		}
		else if (inputCode == operationRestart)
		{
			if (IsStarted())
				Restart();
		}
		else if (inputCode == operationPause)
		{
			if (GameState::Pause == gameState)
				StartResume();
			else if (GameState::Start == gameState)
				Pause();
		}
	}
	else
	{
		if (inputCode == actionLeft || inputCode == actionRight)
		{
			if (IsStarting())
				EndStepHorizontal();
		}
		else if (inputCode == actionRotate)
		{
			if (IsStarting())
				EndRotate();
		}
		else if (inputCode == actionDown)
		{
			if (IsStarting())
			{
				startingDrop = false;
				StartStepDown(false);
			}
		}
		else if (inputCode == actionDrop)
		{
			if (IsStarting())
			{
				if (!dropImmediate)
				{
					startingDrop = false;
					StartStepDown(false);
				}
			}
		}
	}
}

void Controller::Rotate()
{
	if (!IsAvailable())
		return;
	if (pGameFrame->Rotate())
	{
		PlaySnd(MusicType::Rotate);
		InvalidateDraw();
	}
}

void Controller::StepHorizontal(bool left)
{
	if (!IsAvailable())
		return;
	if (left)
	{
		pGameFrame->StepLeft();
	}
	else
	{
		pGameFrame->StepRight();
	}
	PlaySnd(MusicType::StepHorizontal);
	InvalidateDraw();
}

void Controller::StepDown()
{
	if (!IsAvailable())
		return;
	if (pGameFrame->StepDown())
	{
		// dropped
		EndDrop();
	}
	else
	{
		// not dropped
		PlaySnd(MusicType::StepDown);
		InvalidateDraw();
	}
}

void Controller::Drop()
{
	if (!IsAvailable())
		return;
	pGameFrame->Drop();
	EndStepDown();
	EndDrop();
}

void Controller::EndDrop()
{
	PlaySnd(MusicType::Dropped);
	if (pGameFrame->IsFull())
	{
		End();
		return;
	}
	if (pGameFrame->Union() > 0)
	{
		InvalidateDraw();
		PlaySnd(MusicType::Remove);
		StartRemoveBlink();
		return;
	}
	pGameFrame->RebornTetrisShape();
	InvalidateDraw();
	StartStepDown(startingDrop);
}

bool Controller::Start()
{
	if (!IsResourceInitialized())
		return false;
	gameState = GameState::Start;
	InvalidateDraw();
	PlaySnd(MusicType::Bgm);
	StartStepDown(false);
	return true;
}

void Controller::End()
{
	gameState = GameState::End;
	EndStepDown();
	EndRemoveBlink();
	StartRoll();
	InvalidateDraw();
}

void Controller::Pause()
{
	EndStepDown();
	EndRemoveBlink();
	gameState = GameState::Pause;
	InvalidateDraw();
}

void Controller::Resume()
{
	if (pGameFrame->HasFullLine())
	{
		PlaySnd(MusicType::Remove);
		StartRemoveBlink();
	}
	else
	{
		gameState = GameState::Start;
		StartStepDown(false);
	}
}

void Controller::Restart()
{
	End();
}

bool Controller::SaveGame(wstring archive)
{
	if (archive.empty())
		return false;

	Archive::Save(archive, this);

	return true;
}

bool Controller::LoadGame(wstring archive)
{
	if (archive.empty())
		return false;
	End();
	Archive::Load(archive, this);
	Start();
	return true;
}

GameFrame* Controller::GetGameFrame()
{
	return pGameFrame;
}

void Controller::SetBgmOn(bool on)
{
	bgmOn = on;
}

bool Controller::GetBgmOn()
{
	return bgmOn;
}

void Controller::SetSoundOn(bool on)
{
	soundOn = on;
}

bool Controller::GetSoundOn()
{
	return soundOn;
}

void Controller::PlayBgm()
{
	if(bgmOn)	
		pMusician->PostBgmPlay();
}

void Controller::StopBgm()
{
	pMusician->PostBgmStop();
}

bool Controller::StartStepDown(bool isDropping)
{
	startingDrop = isDropping;
	return SetTimer(hWnd, ST_STEPDOWN,
		isDropping ? dropTimespan * 10 : stepDownTimespan * 10,
		StepDownTimerProcStatic);
}

bool Controller::EndStepDown()
{
	if(startingDrop)
		startingDrop = false;
	return KillTimer(hWnd, ST_STEPDOWN);
}

void Controller::StepDownTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.StepDownTimerProc(hWnd, msg, id, millisecond);
}

void Controller::StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	StepDown();
}

bool Controller::StartStepHorizontal(bool stepLeft)
{
	startingStepLeft = stepLeft;
	startingStepRight = !stepLeft;
	return SetTimer(hWnd, ST_STEPHORIZONTAL, stepHorizontalTimespan * 10, StepHorizontalTimerProcStatic);
}

bool Controller::EndStepHorizontal()
{
	if (startingStepLeft)
		startingStepLeft = false;
	else
		startingStepRight = false;
	return KillTimer(hWnd, ST_STEPHORIZONTAL);
}

void Controller::StepHorizontalTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.StepHorizontalTimerProc(hWnd, msg, id, millisecond);
}

void Controller::StepHorizontalTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	StepHorizontal(startingStepLeft);
}

bool Controller::StartRotate()
{
	startingRotate = true;
	return SetTimer(hWnd, ST_ROTATE, rotateTimespan * 10, RotateTimerProcStatic);
}

bool Controller::EndRotate()
{
	startingRotate = false;
	return KillTimer(hWnd, ST_ROTATE);
}

void Controller::RotateTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.RotateTimerProc(hWnd, msg, id, millisecond);
}

void Controller::RotateTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Rotate();
}

bool Controller::StartRemoveBlink()
{
	gameState = GameState::BlinkNormal;
   	removeBlinkTimes = 0;
	return SetTimer(hWnd, ST_REMOVEBLINK, removeBlinkTimespan * 10, RemoveBlinkTimerProcStatic);
}

bool Controller::EndRemoveBlink()
{
	gameState = GameState::Start;
	removeBlinkTimes = 0;
	return KillTimer(hWnd, ST_REMOVEBLINK);
}

void Controller::RemoveBlinkTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.RemoveBlinkTimerProc(hWnd, msg, id, millisecond);
}

void Controller::RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	if (removeBlinkCount == removeBlinkTimes)
	{
		EndRemoveBlink();

		pGameFrame->RemoveFullLines();
		pGameFrame->RebornTetrisShape();
		
		InvalidateDraw();

		StartStepDown(false);
	}
	else
	{
		removeBlinkTimes++;

		gameState = GameState::BlinkNormal == gameState ?
			GameState::BlinkLight : GameState::BlinkNormal;
		
		InvalidateDraw();
	}
}

bool Controller::StartRoll()
{
	gameState = GameState::RollUp;
	pGameFrame->rolledRows = 0;
	return SetTimer(hWnd, ST_ROLL, rollTimespan * 10, RollTimerProcStatic);
}

bool Controller::EndRoll()
{
	gameState = GameState::End;
	pGameFrame->rolledRows = 0;
	return KillTimer(hWnd, ST_ROLL);
}

void Controller::RollTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.RollTimerProc(hWnd, msg, id, millisecond);
}

void Controller::RollTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	if (GameState::RollUp == gameState)
	{
		if (pGameFrame->sizeY == pGameFrame->rolledRows)
		{
			gameState = GameState::RollDown;
			pGameFrame->rolledRows--;
		}
		else
		{
			pGameFrame->rolledRows++;
		}
	}
	else if (GameState::RollDown == gameState)
	{
		if (0 == pGameFrame->rolledRows)
		{
			EndRoll();
			pGameFrame->Reborn();
			Start();
		}
		else
		{
			pGameFrame->rolledRows--;
		}
	}
	InvalidateDraw();
}

bool Controller::StartResume()
{
	gameState = GameState::ResumeDelay;
	InvalidateDraw();
	return SetTimer(hWnd, ST_RESUME, resumeDelayTimespan * 10, ResumeTimerProcStatic);
}

bool Controller::EndResume()
{
	return KillTimer(hWnd, ST_RESUME);
}

void Controller::ResumeTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.ResumeTimerProc(hWnd, msg, id, millisecond);
}

void Controller::ResumeTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	EndResume();
	Resume();
	InvalidateDraw();
}

void Controller::InvalidateDraw()
{
	pDrawer->Invalidate();
}

void Controller::PlaySnd(MusicType musicType)
{
	if (soundOn)
		pMusician->PostSndPlay(musicType);
}
