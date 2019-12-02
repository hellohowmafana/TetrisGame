#include "Musician.hpp"
#include "Configuration.hpp"
#include "Utility.hpp"

Musician Musician::singleton;

void Musician::CreateMusicThread(Configuration* pConfiguration)
{
	hThread = CreateThread(NULL, 0, MusicThreadProc, pConfiguration, 0, 0);
}

void Musician::EndMusicThread()
{
	PostEvent(MusicianEvent::ExitThread, nullptr);
}

DWORD WINAPI Musician::MusicThreadProc(LPVOID pParam)
{
	Musician* pMusician = &Musician::singleton;

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

	// initialize musician
	pMusician->PostInitialize((Configuration*)pParam);

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
			pMusician->Play((MusicType)lParam);
			break;
		case MusicianEvent::Stop:
			pMusician->Stop((MusicType)lParam);
			break;
		case MusicianEvent::Pause:
			pMusician->Pause((MusicType)lParam);
			break;
		case MusicianEvent::Resume:
			pMusician->Resume((MusicType)lParam);
			break;
		case MusicianEvent::ExitThread:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		if(pMusician->musicianCallback)
			pMusician->musicianCallback(pMusician, musicianEvent);
	}
	break;
	case MM_MCINOTIFY:
		if (MCI_NOTIFY_SUCCESSFUL == wParam)
		{
			// bgm playing end
			Musician* pMusician = &Musician::singleton;
			pMusician->ShiftBgm(pMusician->randomBgm, true);
			pMusician->bgm.Play(true);
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
	initialized(false),
	musicianCallback(nullptr)
{
}

Musician::~Musician()
{
	if(initialized)
		Deinitialize();
}

void Musician::InitializeAsync(Configuration* pConfiguration)
{
	CreateMusicThread(pConfiguration);
}

void Musician::DeinitializeAsync()
{
	EndMusicThread();
}

void Musician::SetCallback(MusicianCallback musicianCallback)
{
	this->musicianCallback = musicianCallback;
}

void Musician::PostInitialize(Configuration* pConfiguration)
{
	PostEvent(MusicianEvent::Initialize, pConfiguration);
}

void Musician::PostDeinitialize()
{
	PostEvent(MusicianEvent::Deinitialize, nullptr);
}

void Musician::PostPlay(MusicType musicType)
{
	PostEvent(MusicianEvent::Play, (void*)musicType);
}

void Musician::PostStop(MusicType musicType)
{
	PostEvent(MusicianEvent::Stop, (void*)musicType);
}

void Musician::PostPause(MusicType musicType)
{
	PostEvent(MusicianEvent::Pause, (void*)musicType);
}

void Musician::PostResume(MusicType musicType)
{
	PostEvent(MusicianEvent::Resume, (void*)musicType);
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

	// open sounds
	CreateSound(MusicType::StepDown)->Open();
	CreateSound(MusicType::StepHorizontal)->Open();
	CreateSound(MusicType::Rotate)->Open();
	CreateSound(MusicType::Dropped)->Open();
	CreateSound(MusicType::Remove)->Open();

	// open bgm
	randomBgm = pConfiguration->randomBgm;
	bgm.SetNotifyWindow(hWnd);
	bgm.SetMusic(MusicType::Bgm, bgmAlias, L"");
	ShiftBgm(randomBgm, true);

	initialized = true;
}

void Musician::Deinitialize()
{
	if (!initialized) return;

	// close all
	CloseAll();

	// clear
	mapSoundPaths.clear();
	vecBgms.clear();
	ClearSounds();
	currentBgm = -1;

	initialized = false;
}

bool Musician::Play(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return bgm.Play(true);
	else
		return GetPlayableSound(musicType)->Play(false);
}

bool Musician::Stop(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return bgm.Stop();
	else
		return StopSound(musicType);
}

bool Musician::Pause(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return bgm.Pause();
	return false;
}

bool Musician::Resume(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return bgm.Resume();
	return false;
}

bool Musician::CloseAll()
{
	wstring mciString;
	mciString.append(L"close all");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Music* Musician::CreateSound(MusicType musicType)
{
	if (MusicType::Bgm == musicType)
		return nullptr;

	Music* pMusic = new Music(musicType, mapSoundPaths[musicType].c_str(), CreateSoundAlias().c_str());
	sounds[musicType].push_back(pMusic);
	return pMusic;
}

bool Musician::DeleteSound(Music* pMusic)
{
	if (MusicType::Bgm == pMusic->GetType())
		return false;

	for (SoundGroup::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		SoundClass* pSoundClass = &it->second;
		SoundClass::iterator itc = find(pSoundClass->begin(), pSoundClass->end(), pMusic);
		if (pSoundClass->end() != itc)
		{
			DeleteAlias((*itc)->GetAlias());
			delete* itc;
			pSoundClass->erase(itc);
			return true;
		}
	}
	return false;
}

void Musician::ClearSounds()
{
	for (SoundGroup::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		for (SoundClass::iterator itc = it->second.begin(); itc != it->second.end(); itc++)
		{
			delete* itc;
		}
	}
	sounds.clear();
}

Music* Musician::FindSound(MCIDEVICEID deviceId)
{
	for (SoundGroup::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		SoundClass* pSoundClass = &it->second;
		for (SoundClass::iterator itc = pSoundClass->begin(); itc != pSoundClass->end(); itc++)
		{
			if (deviceId == (*itc)->GetDeviceId())
				return *itc;
		}
	}
	return nullptr;
}

Music* Musician::GetPlayableSound(MusicType musicType)
{
	// find exist playable sound
	SoundClass* pMusicianClass = &sounds[musicType];
	for (SoundClass::iterator it = pMusicianClass->begin(); it != pMusicianClass->end(); it++)
	{
		if (MusicStatus::Stop == (*it)->GetStatus())
		{
			return *it;
		}
	}

	// create and open one sound
	Music* pMusic = CreateSound(musicType);
	pMusic->Open();
	return pMusic;
}

bool Musician::StopSound(MusicType musicType)
{
	bool result = true;
	for (SoundClass::iterator it = sounds[musicType].begin(); it != sounds[musicType].end(); it++)
	{
		result = result && (*it)->Close();
	}
	return result;
}

wstring Musician::CreateSoundAlias()
{
	int i = 0;
	while (true)
	{
		if (setSoundAliases.end() == setSoundAliases.find(i))
			break;
		i++;
	}
	setSoundAliases.emplace(i);
	return to_wstring(i);
}

bool Musician::DeleteAlias(wstring alias)
{
	return 0 != setSoundAliases.erase(stoi(alias));
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
