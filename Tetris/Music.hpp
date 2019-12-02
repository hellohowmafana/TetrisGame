#pragma once
#include <string>
#include <Windows.h>
#include <mciapi.h>
using namespace std;
#pragma comment (lib,"Winmm.lib")

enum class MusicStatus { Play, Close, Stop, Pause };

enum class MusicType { StepDown, StepHorizontal, Rotate, Dropped, Remove, Bgm };

class Music
{
public:
	Music();
	Music(MusicType musicType, wstring musicPath, wstring alias);
	~Music();
	void SetMusic(MusicType musicType, wstring alias, wstring musicPath);
	void SetNotifyWindow(HWND hWnd);

	bool Open();
	bool Close();
	bool Play(bool notify);
	bool Stop();
	bool Pause();
	bool Resume();
	bool Shift(wstring musicPath, bool forceClose, bool open);

	MusicType GetType();
	wstring GetAlias();
	MusicStatus GetStatus();
	MCIDEVICEID GetDeviceId();
	bool IsSound();
	bool IsBgm();

protected:
	HWND hWndMsg;
	MusicType musicType;
	wstring musicPath;
	wstring alias;
	MCIDEVICEID deviceId;
};
