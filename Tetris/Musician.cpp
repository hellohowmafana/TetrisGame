#include "Musician.hpp"
#include "Configuration.hpp"
#include "Utility.hpp"

Musician Musician::singleton;

bool Musician::CreateMusicThread()
{
	hevThreadStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hevThreadStarted == NULL)
	{
		return false;
	}

	hThread = CreateThread(NULL, 0, MusicThreadProc, this, 0, 0);
	if (hThread == NULL)
	{
		CloseHandle(hevThreadStarted);
		return false;
	}

	WaitForSingleObject(hevThreadStarted, INFINITE);
	CloseHandle(hevThreadStarted);
	hevThreadStarted = nullptr;
	return true;
}

void Musician::EndMusicThread()
{
	PostEvent(MusicianEvent::ExitThread, nullptr);
}

DWORD WINAPI Musician::MusicThreadProc(LPVOID pParam)
{
	Musician* pMusician = (Musician*)pParam;

	// create message window
	wchar_t* szWindowClass = (wchar_t*)L"MessageMusician";
	WNDCLASS wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASS));
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpszClassName = szWindowClass;
	if (RegisterClass(&wndClass))
		pMusician->hWnd = CreateWindow(szWindowClass, nullptr, 0, 0, 0, 0, 0,
			HWND_MESSAGE, NULL, GetModuleHandle(NULL), 0);
	
	// signal the thread has started
	SetEvent(pMusician->hevThreadStarted);

	// dispatch messages
	MSG msg;
	BOOL bRet;
	while (bRet = GetMessage(&msg, pMusician->hWnd, 0, 0))
	{
		if (bRet != -1)
		{
			DispatchMessage(&msg);
		}
	}

	return 0;
}

LRESULT CALLBACK Musician::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case UM_MUSICIAN:
	{
		Musician* pMusician = &Musician::singleton;
		MusicianEvent musicianEvent = (MusicianEvent)wParam;
		switch (musicianEvent)
		{
		case MusicianEvent::Initialize:
			pMusician->Initialize((Configuration*)lParam);
			break;
		case MusicianEvent::Deinitialize:
			pMusician->Deinitialize();
			break;
		case MusicianEvent::Play:
		{
			MusicType musicType = (MusicType)lParam;
			if (MusicType::Bgm == musicType)
				pMusician->PlayBgm();
			else
				pMusician->PlaySnd(musicType);
			break;
		}
		case MusicianEvent::Stop:
			if(MusicType::Bgm == (MusicType)lParam) pMusician->StopBgm();
			break;
		case MusicianEvent::Pause:
			if (MusicType::Bgm == (MusicType)lParam) pMusician->PauseBgm();
			break;
		case MusicianEvent::Resume:
			if (MusicType::Bgm == (MusicType)lParam) pMusician->ResumeBgm();
			break;
		case MusicianEvent::ExitThread:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	break;
	case MM_MCINOTIFY:
		if (MCI_NOTIFY_SUCCESSFUL == wParam)
		{
			Musician* pMusician = &Musician::singleton;
			MCIDEVICEID devId = (MCIDEVICEID)lParam;
			if (devId == Musician::singleton.bgm.GetDeviceId())
			{
				// bgm playing end
				pMusician->ShiftBgm(pMusician->randomBgm, true);
				pMusician->bgm.Play(true);
			}
			else
			{
				// sound playback end
				pMusician->CloseSnd(devId);
			}
		}
		break;
	case WM_QUIT:
		Musician::singleton.Deinitialize();
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void Musician::PostEvent(MusicianEvent musicianEvent, void* param)
{
	PostMessage(hWnd, UM_MUSICIAN, (WPARAM)musicianEvent, (LPARAM)param);
}

Musician::Musician() :
	currentBgm(-1),
	initialized(false)
{
	hevInitialized = CreateEvent(NULL, TRUE, FALSE, NULL);
}

Musician::~Musician()
{
	if(initialized)
		Deinitialize();
	if(hevInitialized)
		CloseHandle(hevInitialized);
}

bool Musician::InitializeInNewThread(Configuration* pConfiguration)
{
	bool success =
	CreateMusicThread();
	PostInitialize(pConfiguration);
	WaitForSingleObject(hevInitialized, INFINITE);
	return success;
}

void Musician::DeinitializeAndDestroyThread()
{
	PostDeinitialize();
	EndMusicThread();
}

void Musician::PostInitialize(Configuration* pConfiguration)
{
	PostEvent(MusicianEvent::Initialize, pConfiguration);
}

void Musician::PostDeinitialize()
{
	PostEvent(MusicianEvent::Deinitialize, nullptr);
}

void Musician::PostBgmPlay()
{
	PostEvent(MusicianEvent::Play, (void*)MusicType::Bgm);
}

void Musician::PostBgmStop()
{
	PostEvent(MusicianEvent::Stop, (void*)MusicType::Bgm);
}

void Musician::PostBgmPause()
{
	PostEvent(MusicianEvent::Pause, (void*)MusicType::Bgm);
}

void Musician::PostBgmResume()
{
	PostEvent(MusicianEvent::Resume, (void*)MusicType::Bgm);
}

void Musician::PostSndPlay(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return;
	PostEvent(MusicianEvent::Play, (void*)musicType);
}

bool Musician::IsInitialized()
{
	return initialized;
}

void Musician::Initialize(Configuration* pConfiguration)
{
	if (initialized) return;

	// save music paths
	mapSoundPaths[MusicType::StepDown] = pConfiguration->pathStepDownSound;
	mapSoundPaths[MusicType::StepHorizontal] = pConfiguration->pathStepHorizontalSound;
	mapSoundPaths[MusicType::Rotate] = pConfiguration->pathRotateSound;
	mapSoundPaths[MusicType::Dropped] = pConfiguration->pathDroppedSound;
	mapSoundPaths[MusicType::Remove] = pConfiguration->pathRemoveSound;
	vecBgms = pConfiguration->vecBgms;
	sort(vecBgms.begin(), vecBgms.end());
	
	// soundAlias
	soundAlias = 0;

	// open bgm
	randomBgm = pConfiguration->randomBgm;
	bgm.SetNotifyWindow(hWnd);
	bgm.SetMusic(MusicType::Bgm, bgmAlias, L"");
	ShiftBgm(randomBgm, true);

	initialized = true;
	if (hevInitialized)
		SetEvent(hevInitialized);
}

void Musician::Deinitialize()
{
	if (!initialized) return;

	// close all
	CloseAll();

	// clear
	mapSoundPaths.clear();
	vecBgms.clear();
	currentBgm = -1;

	initialized = false;
	if (hevInitialized)
		ResetEvent(hevInitialized);
}

Music* Musician::ShiftBgm(bool random, bool open)
{
	if (vecBgms.empty())
		return nullptr;

	if (1 == vecBgms.size())
		currentBgm = 0;
	else
	{
		if (random)
		{
			if (-1 == currentBgm)
				currentBgm = Utility::Random(0, (int)vecBgms.size() - 1);
			else
			{
				int random = Utility::Random(0, (int)vecBgms.size() - 2);
				if (random == currentBgm)
					random = (int)vecBgms.size() - 1;
				currentBgm = random;
			}
		}
		else
		{
			currentBgm++;
			currentBgm %= vecBgms.size();
		}
	}

	bgm.Shift(vecBgms[currentBgm], true, open);
	return &bgm;
}

bool Musician::PlayBgm()
{
	return bgm.Play(true);
}

bool Musician::StopBgm()
{
	return bgm.Stop();
}

bool Musician::PauseBgm()
{
	return bgm.Pause();
}

bool Musician::ResumeBgm()
{
	return bgm.Resume();
}

bool Musician::PlaySnd(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return false;
	Music* sound = new Music(musicType, mapSoundPaths[musicType], to_wstring(soundAlias++));
	sounds[musicType].push_back(sound);
	sound->SetNotifyWindow(hWnd);
	return sound->Open() && sound->Play(true);
}

bool Musician::CloseSnd(MCIDEVICEID deviceId)
{
	for (SoundSet::iterator its = sounds.begin(); its != sounds.end(); its++)
	{
		auto it = std::find_if((*its).second.begin(), (*its).second.end(),
			[&](const Music* sound) {
				return sound->GetDeviceId() == deviceId;
			});
		if (it != (*its).second.end())
		{
			(*it)->Close();
			delete (*it);
			(*its).second.erase(it);
			return true;
		}
	}
	return false;
}

bool Musician::CloseAll()
{
	wstring mciString;
	mciString.append(L"close all");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		for (SoundSet::iterator its = sounds.begin(); its != sounds.end(); its++)
		{
			for (SoundGroup::iterator itg = (*its).second.begin(); itg != (*its).second.end(); itg++)
			{
				delete* itg;
			}
		}
		sounds.clear();
		return true;
	}
	else
	{
		return false;
	}
}
