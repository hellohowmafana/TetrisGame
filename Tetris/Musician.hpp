#pragma once
#include "Music.hpp"
#include <Windows.h>
#include <map>
#include <set>
#include <vector>

class Configuration;
class Musician;

#define UM_MUSICIAN WM_USER

enum class MusicianEvent { Initialize, Deinitialize, Play, Stop, Pause, Resume, ExitThread };

typedef void (CALLBACK* MusicianCallback)(Musician* pMusician, MusicianEvent musicianEvent);

class Musician
{
public:
	static Musician singleton;

private:
	bool CreateMusicThread();
	void EndMusicThread();
	static DWORD WINAPI MusicThreadProc(LPVOID param);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void PostEvent(MusicianEvent musicianEvent, void* param);
	
	HANDLE hThread;
	HANDLE hevThreadStarted;
	HANDLE hevInitialized;
	HWND hWnd;
	bool initialized;
	
	Musician();
	~Musician();

public:
	bool InitializeInNewThread(Configuration* pConfiguration);
	void DeinitializeAndDestroyThread();

	// async methods
	void PostInitialize(Configuration* pConfiguration);
	void PostDeinitialize();
	void PostBgmPlay();
	void PostBgmStop();
	void PostBgmPause();
	void PostBgmResume();
	void PostSndPlay(MusicType musicType);

	// sync methods
	bool IsInitialized();
	void Initialize(Configuration* pConfiguration);
	void Deinitialize();

	Music* ShiftBgm(bool random, bool open);
	bool PlayBgm();
	bool StopBgm();
	bool PauseBgm();
	bool ResumeBgm();

	bool PlaySnd(MusicType musicType);
	bool CloseSnd(MCIDEVICEID deviceId);

	bool CloseAll();

private:
	// sound
	typedef vector<Music*> SoundGroup;
	typedef map<MusicType, SoundGroup> SoundSet;
	SoundSet sounds;
	map<MusicType, wstring> mapSoundPaths;
	int soundAlias;

private:
	// bgm
	int currentBgm;
	bool randomBgm;
	vector<wstring> vecBgms;

	Music bgm;
	wstring bgmAlias = L"bgm";
};
