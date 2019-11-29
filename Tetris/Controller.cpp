#include "Controller.h"
#include "Configuration.h"
#include "Level.h"
#include "Archive.h"
#include "Drawer.h"
#include "Musician.h"

Controller Controller::singleton;

void Controller::Initialize(Configuration* pConfiguration)
{
	this->dropImmediate = pConfiguration->dropImmediate;

	this->stepDownTimespan = Level::GetLevel(pConfiguration->startLevel)->stepDownTimeSpan;
	this->dropTimespan = pConfiguration->dropTimespan;
	this->removeBlinkTimespan = pConfiguration->removeBlinkTimespan;
	this->removeBlinkCount = pConfiguration->removeBlinkCount;
	removeBlinkTimes = 0;
	this->rollTimespan = pConfiguration->rollTimespan;
	this->resumeDelayTimespan = pConfiguration->resumeDelayTimespan;

	soundOn = pConfiguration->soundOn;
	bgmOn = pConfiguration->bgmOn;

	initialized = true;
	gameState = GameState::None;
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
	pMusician->SetCallback(MusicianCallbackStatic);
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

void Controller::KeyDownAction(WPARAM keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		StepHorizontal(true);
		break;
	case VK_RIGHT:
		StepHorizontal(false);
		break;
	case VK_DOWN:
		StepDown();
		break;
	case VK_UP:
		Rotate();
		break;
	case VK_SPACE:
		dropImmediate ? Drop() : StepDown();
		break;
	case VK_RETURN:
		Restart();
		break;
	case 'P':
		if (GameState::Pause == gameState)
			StartResume();
		else if(GameState::Start == gameState)
			Pause();
		break;
	default:
		break;
	}
	InvalidateDraw();
}

void Controller::KeyUpAction(WPARAM keyCode)
{
}

void Controller::Rotate()
{
	if (GameState::Start != gameState)
		return;
	pGameFrame->Rotate();
	PlayMusic(MusicType::Rotate);
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
	PlayMusic(MusicType::StepHorizontal);
}

void Controller::StepDown()
{
	if (GameState::Start != gameState)
		return;
	if (pGameFrame->StepDown())
	{
		// dropped
		EndStepDown();
		EndDrop();
	}
	else
	{
		// not dropped
		PlayMusic(MusicType::StepDown);
	}
}

void Controller::Drop()
{
	if (GameState::Start != gameState)
		return;
	pGameFrame->Drop();
	EndStepDown();
	EndDrop();
}

void Controller::EndDrop()
{
	PlayMusic(MusicType::Dropped);
	if (pGameFrame->Union() > 0)
	{
		InvalidateDraw();
		PlayMusic(MusicType::Remove);
		StartRemoveBlink();
		return;
	}
	if (pGameFrame->IsFull())
	{
		End();
		return;
	}
	pGameFrame->RebornTetrisShape();
	InvalidateDraw();
	StartStepDown(false);
}

bool Controller::Start()
{
	if (!IsResourceInitialized())
		return false;
	gameState = GameState::Start;
	InvalidateDraw();
	PlayMusic(MusicType::Bgm);
	StartStepDown(false);
	return true;
}

void Controller::End()
{
	gameState = GameState::End;
	EndStepDown();
	EndRemoveBlink();
	StartRoll();
}

void Controller::Pause()
{
	EndStepDown();
	EndRemoveBlink();
	gameState = GameState::Pause;
}

void Controller::Resume()
{
	if (pGameFrame->HasFullLine())
	{
		PlayMusic(MusicType::Remove);
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
	Start();
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
	PlayMusic(MusicType::Bgm);
}

void Controller::StopBgm()
{
	StopMusic(MusicType::Bgm);
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
	InvalidateDraw();
}

bool Controller::StartRemoveBlink()
{
	gameState = GameState::BlinkNormal;
   	removeBlinkTimes = 0;
	return SetTimer(hWnd, ST_REMOVEBLINK, removeBlinkTimespan, RemoveBlinkTimerProcStatic);
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
	return SetTimer(hWnd, ST_ROLL, rollTimespan, RollTimerProcStatic);
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
	gameState = GameState::ResumeDelay;
	InvalidateDraw();
	return SetTimer(hWnd, ST_RESUME, resumeDelayTimespan, ResumeTimerProcStatic);
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

void Controller::PlayMusic(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
	{
		if (bgmOn)
			pMusician->PostPlay(musicType);
	}
	else
	{
		if (soundOn)
			pMusician->PostPlay(musicType);
	}
}

void Controller::StopMusic(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		pMusician->PostStop(musicType);
}

void CALLBACK Controller::MusicianCallbackStatic(Musician* pMusician, MusicianEvent musicianEvent)
{
	Controller::singleton.MusicianCallback(pMusician, musicianEvent);
}

void Controller::MusicianCallback(Musician* pMusician, MusicianEvent musicianEvent)
{
	if (pMusician == GetMusician())
		if (MusicianEvent::Initialize == musicianEvent)
			Start();
}
