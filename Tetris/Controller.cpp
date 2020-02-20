#include "Controller.hpp"
#include "Configuration.hpp"
#include "Level.hpp"
#include "Archive.hpp"
#include "Drawer.hpp"
#include "Musician.hpp"
#include "Recorder.hpp"

Controller Controller::singleton;

bool Controller::OnUpdate(Configuration* pConfiguration)
{
	this->pConfiguration = pConfiguration;

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
	this->shapeBlinkTimespan = pConfiguration->shapeBlinkTimespan;

	this->soundOn = pConfiguration->soundOn;
	this->bgmOn = pConfiguration->bgmOn;
	this->record = pConfiguration->record;

	gameState = GameState::None;

	return true;
}

bool Controller::OnDeinitialize()
{
	this->pConfiguration = nullptr;
	return true;
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

void Controller::SetRecorder(Recorder* pRecorder)
{
	this->pRecorder = pRecorder;
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
		GameState::ResourceInitialized != gameState &&
		GameState::End != gameState &&
		GameState::RollUp != gameState &&
		GameState::RollDown != gameState;
}

bool Controller::IsStarting()
{
	return GameState::Start == gameState;
}

bool Controller::IsShapeLighting()
{
	return isShapeLighting;
}

void Controller::OnKeyDown(WPARAM keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		if (IsStarting())
		{
			if (!startingStepLeft)
			{
				StepHorizontal(true);
				StartStepHorizontal(true);
			}
		}
		break;
	case VK_RIGHT:
		if (IsStarting())
		{
			if (!startingStepRight)
			{
				StepHorizontal(false);
				StartStepHorizontal(false);
			}
		}
		break;
	case VK_UP:
		if (IsStarting())
		{
			if (!startingRotate)
			{
				Rotate();
				StartRotate();
			}
		}
		break;
	case VK_DOWN:
	case VK_SPACE:
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
		break;
	case VK_RETURN:
		if (IsStarted())
			Restart();
		else
		{
			InitializeGame();
			Start();
		}
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
}

void Controller::OnKeyUp(WPARAM keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		if (IsStarting())
			EndStepHorizontal();
		break;
	case VK_RIGHT:
		if (IsStarting())
			EndStepHorizontal();
		break;
	case VK_UP:
		if (IsStarting())
			EndRotate();
		break;
	case VK_DOWN:
	case VK_SPACE:
		if (IsStarting())
		{
			if (!dropImmediate)
			{
				startingDrop = false;
				StartStepDown(false);
			}
		}
		break;
	default:
		break;
	}
}

void Controller::Rotate()
{
	if (!IsStarting())
		return;
	if (pGameFrame->Rotate())
	{
		PlayMusic(MusicType::Rotate);
		InvalidateDraw();
	}
}

void Controller::StepHorizontal(bool left)
{
	if (!IsStarting())
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
	InvalidateDraw();
}

void Controller::StepDown()
{
	if (!IsStarting())
		return;
	if (pGameFrame->StepDown())
	{
		// dropped
		EndStepDown();
		FinishDrop();
	}
	else
	{
		// not dropped
		PlayMusic(MusicType::StepDown);
		InvalidateDraw();
	}
}

void Controller::Drop()
{
	if (!IsStarting())
		return;
	pGameFrame->Drop();
	EndStepDown();
	FinishDrop();
}

void Controller::FinishDrop()
{
	if (pGameFrame->GetShape()->IsPenerable())
		EndShapeBlink();
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
	if (pGameFrame->GetShape()->IsPenerable())
		StartShapeBlink();
	InvalidateDraw();
	StartStepDown(startingDrop);
}

bool Controller::Start()
{		
	if (!IsResourceInitialized())
		return false;

	if(!IsReplay())
		if(record)
			StartRecord();

	gameState = GameState::Start;
	isShapeLighting = false;
	if (pGameFrame->GetShape()->IsPenerable())
		StartShapeBlink();
	InvalidateDraw();
	PlayMusic(MusicType::Bgm);
	StartStepDown(false);
	return true;
}

void Controller::End()
{
	if (record)
		EndRecord();

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
		PlayMusic(MusicType::Remove);
		StartRemoveBlink();
	}
	else
	{
		gameState = GameState::Start;
		StartStepDown(false);
	}
}

void Controller::InitializeGame()
{
	mode = Mode::Normal;
	pGameFrame->InitializeGame();
}

void Controller::Restart()
{
	End();
	if (Mode::Normal == mode)
	{
		pGameFrame->Reborn();
	}
	else
	{
		pConfiguration->Restore();

	}
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
	mode = Mode::Normal;
	Start();
	return true;
}

bool Controller::StartRecord()
{
	wchar_t file[31];
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);
	wsprintf(file, L"record_%04d-%02d-%02d_%02d-%02d-%02d.rec",
		systemtime.wYear,
		systemtime.wMonth,
		systemtime.wDay,
		systemtime.wHour,
		systemtime.wMinute,
		systemtime.wSecond
	);
	return pRecorder->StartRecord(file, pConfiguration, this);
}

bool Controller::EndRecord()
{
	return pRecorder->EndRecord();
}

bool Controller::PlayRecord(wstring record)
{
	mode = Mode::Replay;

	if(!pRecorder->Load(record, pConfiguration, this))
		return false;

	if (!Start())
		return false;

	return true;
}

bool Controller::IsReplay()
{
	return Mode::Replay == mode;
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
		FinishRemoveBlink();
	}
	else
	{
		removeBlinkTimes++;

		gameState = GameState::BlinkNormal == gameState ?
			GameState::BlinkLight : GameState::BlinkNormal;
		
		InvalidateDraw();
	}
}

void Controller::FinishRemoveBlink()
{
	pGameFrame->RemoveFullLines();
	pGameFrame->RebornTetrisShape();
	if (pGameFrame->GetShape()->IsPenerable())
		StartShapeBlink();
	InvalidateDraw();
	StartStepDown(false);
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

bool Controller::StartShapeBlink()
{
	isShapeLighting = true;
	InvalidateDraw();
	return SetTimer(hWnd, ST_SHAPEBLINK, shapeBlinkTimespan * 10, ShapeBlinkTimerProcStatic);
}

bool Controller::EndShapeBlink()
{
	isShapeLighting = false;
	InvalidateDraw();
	return KillTimer(hWnd, ST_SHAPEBLINK);
}

void Controller::ShapeBlinkTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	Controller::singleton.ShapeBlinkTimerProc(hWnd, msg, id, millisecond);
}

void Controller::ShapeBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond)
{
	isShapeLighting = !isShapeLighting;
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
		{
			gameState = GameState::ResourceInitialized;
			InvalidateDraw();
		}
}
