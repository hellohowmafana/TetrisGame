#pragma once
#include "Music.hpp"
#include <Windows.h>
#include <map>
#include <set>
#include <vector>
#include "Configurable.hpp"

class Configuration;
class Musician;

#define UM_MUSICIAN WM_USER + 1

enum class MusicianEvent { Initialize, Deinitialize, Play, Stop, Pause, Resume, ExitThread };

typedef void (CALLBACK* MusicianCallback)(Musician* pMusician, MusicianEvent musicianEvent);

class Musician : public Configurable
{
public:
	static Musician singleton;

private:
	void CreateMusicThread(Configuration* pConfiguration);
	void EndMusicThread();
	static DWORD WINAPI MusicThreadProc(LPVOID param);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void PostEvent(MusicianEvent musicianEvent, void* param);
	MusicianCallback musicianCallback;
	HANDLE hThread;
	HWND hWnd;
	bool initialized;

	Musician();
	~Musician();

public:
	// async methods
	void InitializeAsync(Configuration* pConfiguration);
	void DeinitializeAsync();
	void SetCallback(MusicianCallback musicianCallback);

	void PostInitialize(Configuration* pConfiguration);
	void PostDeinitialize();
	void PostPlay(MusicType musicType);
	void PostStop(MusicType musicType);
	void PostPause(MusicType musicType);
	void PostResume(MusicType musicType);

	// sync methods
	bool OnUpdate(Configuration* pConfiguration);
	bool OnDeinitialize();

	bool Play(MusicType musicType);
	bool Stop(MusicType musicType);
	bool Pause(MusicType musicType);
	bool Resume(MusicType musicType);

	bool CloseAll();

private:
	// sound
	map<MusicType, wstring> mapSoundPaths;

	// for overlapped playing
	typedef vector<Music*> SoundClass;
	typedef map<MusicType, SoundClass> SoundGroup;
	SoundGroup sounds;

	Music* CreateSound(MusicType musicType);
	bool DeleteSound(Music* pMusic);
	void ClearSounds();
	Music* FindSound(MCIDEVICEID deviceId);
	Music* GetPlayableSound(MusicType musicType);
	bool StopSound(MusicType musicType);

	// aliases
	set<int> setSoundAliases;
	wstring CreateSoundAlias();
	bool DeleteAlias(wstring alias);

private:
	// bgm
	int currentBgm;
	bool randomBgm;
	vector<wstring> vecBgms;
	Music* ShiftBgm(bool random, bool open);

	Music bgm;
	wstring bgmAlias = L"bgm";
};
