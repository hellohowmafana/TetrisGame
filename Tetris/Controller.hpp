#pragma once
#include "GameFrame.hpp"
#include "Musician.hpp"

class Drawer;

enum class GameState
{
	None, Start, Pause, End, ResumeDelay, BlinkLight, BlinkNormal, RollUp, RollDown
};

class Controller
{
public:
	static Controller singleton;
	void Initialize(Configuration* pConfiguration);
	void SetHWnd(HWND hWnd);
	void SetGameFrame(GameFrame* pGameFrame);

	void SetDrawer(Drawer* pDrawer);
	Drawer* GetDrawer();
	void SetMusician(Musician* pMusician);
	Musician* GetMusician();

	bool IsInitialized();
	bool IsResourceInitialized();
	GameState GetGameState();
	bool IsStarted();
	bool IsStarting();

	void OnKeyDown(WPARAM keyCode);
	void OnKeyUp(WPARAM keyCode);

	void Rotate();
	void StepHorizontal(bool left);
	void StepDown();
	void Drop();
	void EndDrop();

	bool Start();
	void End();
	void Pause();
	void Resume();
	void Restart();

	bool SaveGame(wstring archive);
	bool LoadGame(wstring archive);

public:
	GameFrame* GetGameFrame();
	void SetBgmOn(bool on);
	bool GetBgmOn();
	void SetSoundOn(bool on);
	bool GetSoundOn();

	void PlayBgm();
	void StopBgm();

private:
	const UINT_PTR ST_STEPDOWN = 1;
	const UINT_PTR ST_STEPHORIZONTAL = 2;
	const UINT_PTR ST_ROTATE = 3;
	const UINT_PTR ST_REMOVEBLINK = 4;
	const UINT_PTR ST_ROLL = 5;
	const UINT_PTR ST_RESUME = 6;

	int removeBlinkCount;
	int removeBlinkTimes;
	
	bool StartStepDown(bool isDropping);
	bool EndStepDown();
	static void CALLBACK StepDownTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartStepHorizontal(bool stepLeft);
	bool EndStepHorizontal();
	static void CALLBACK StepHorizontalTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void StepHorizontalTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartRotate();
	bool EndRotate();
	static void CALLBACK RotateTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void RotateTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartRemoveBlink();
	bool EndRemoveBlink();
	static void CALLBACK RemoveBlinkTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartRoll();
	bool EndRoll();
	static void CALLBACK RollTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void RollTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartResume();
	bool EndResume();
	static void CALLBACK ResumeTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void ResumeTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

private:
	Controller() {};
	void InvalidateDraw();
	void PlayMusic(MusicType musicType);
	void StopMusic(MusicType musicType);
	static void CALLBACK MusicianCallbackStatic(Musician* pMusician, MusicianEvent musicianEvent);
	void MusicianCallback(Musician* pMusician, MusicianEvent musicianEvent);

	HWND hWnd;
	GameFrame* pGameFrame;
	Drawer* pDrawer;
	Musician* pMusician;
	bool initialized;
	GameState gameState;
	bool startingDrop;
	bool startingStepLeft;
	bool startingStepRight;
	bool startingRotate;

	bool dropImmediate;

	UINT stepDownTimespan;
	UINT dropTimespan;
	UINT stepHorizontalTimespan;
	UINT rotateTimespan;
	UINT removeBlinkTimespan;
	UINT rollTimespan;
	UINT resumeDelayTimespan;

	bool soundOn;
	bool bgmOn;
};

