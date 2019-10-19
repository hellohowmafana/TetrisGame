#pragma once
#include "GameFrame.h"

enum class GameState
{
	Start, Stop, Pause, Resume
};

class Controller
{
public:
	static Controller singleton;
	void Initialize(Configuration* pConfiguration);
	void SetHWnd(HWND hWnd);
	void SetGameFrame(GameFrame* pGameFrame);

	void Rotate();
	void StepHorizontal(bool left);
	void StepDown();
	void Drop();

	void Start();
	void Stop();
	void Pause();
	void Resume();

	bool IsStarted();

	bool SaveGame(TCHAR* szArchive);
	bool LoadGame(TCHAR* szArchive);

public:
	GameFrame* GetGameFrame();
	TetrisShape* GetTetrisShape();
	TetrisShape* GetNextTetrisShape();
	Mass* GetMass();

private:
	const UINT_PTR ST_STEPDOWN = 1;
	const UINT_PTR ST_DROP = 2;
	const UINT_PTR ST_DROPDELAY = 3;
	const UINT_PTR ST_REMOVEDELAY = 4;
	const UINT_PTR ST_REMOVEBLINK = 5;
	
	void SetStepDownTimer();
	void KillStepDownTimer();
	static void CALLBACK StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	void SetDropTimer();
	void KillDropTimer();
	static void CALLBACK DropTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	void SetDropDelayTimer();
	void KillDropDelayTimer();
	static void CALLBACK DropDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	void SetRemoveDelayTimer();
	void KillRemoveDelayTimer();
	static void CALLBACK RemoveDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	void SetRemoveBlinkTimer();
	void KillRemoveBlinkTimer();
	static void CALLBACK RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

private:
	Controller() {};
	HWND hWnd;
	GameFrame* pGameFrame;
	GameState gameState;

	UINT stepDownTimespan;
	UINT dropTimespan;
	UINT dropDelayTimespan;
	UINT removeDelayTimespan;
	UINT removeBlinkTimespan;
};

