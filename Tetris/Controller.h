#pragma once
#include "GameFrame.h"
#include "Musician.h"

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
	GameState GetGameState();
	bool IsStarted();

	void KeyDownAction(WPARAM keyCode);
	void KeyUpAction(WPARAM keyCode);

	void Rotate();
	void StepHorizontal(bool left);
	void StepDown();
	void Drop();
	void EndDrop();

	void Start();
	void End();
	void Pause();
	void Resume();
	void Restart();

	bool SaveGame(wchar_t* szArchive);
	bool LoadGame(wchar_t* szArchive);

public:
	GameFrame* GetGameFrame();
	TetrisShape* GetTetrisShape();
	TetrisShape* GetNextTetrisShape();
	Mass* GetMass();

private:
	const UINT_PTR ST_STEPDOWN = 1;
	const UINT_PTR ST_REMOVEBLINK = 2;
	const UINT_PTR ST_ROLL = 3;
	const UINT_PTR ST_RESUME = 4;

	int removeBlinkCount;
	int removeBlinkTimes;
	
	bool StartStepDown(bool isDropping);
	bool EndStepDown();
	static void CALLBACK StepDownTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

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
	static void CALLBACK MusicianCallbackStatic(Musician* pMusician, MusicianEvent musicianEvent);
	void MusicianCallback(Musician* pMusician, MusicianEvent musicianEvent);

	HWND hWnd;
	GameFrame* pGameFrame;
	Drawer* pDrawer;
	Musician* pMusician;
	bool initialized;
	GameState gameState;

	bool dropImmediate;

	UINT stepDownTimespan;
	UINT dropTimespan;
	UINT dropDelayTimespan;
	UINT removeBlinkTimespan;
	UINT rollTimespan;
	UINT resumeDelayTimespan;

	bool soundOn;
	bool bgmOn;
};

