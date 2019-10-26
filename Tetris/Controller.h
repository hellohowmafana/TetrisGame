#pragma once
#include "GameFrame.h"
#include "Drawer.h"

enum class GameState
{
	None, Start, Pause, End
};

class Controller
{
public:
	static Controller singleton;
	void Initialize(Configuration* pConfiguration);
	void SetHWnd(HWND hWnd);
	void SetGameFrame(GameFrame* pGameFrame);
	void SetDrawer(Drawer* pDrawer);
	bool IsInitialized();
	GameState GetGameState();

	void KeyDownAction(WPARAM keyCode);
	void KeyUpAction(WPARAM keyCode);

	void Rotate();
	void StepHorizontal(bool left);
	void StepDown();
	void Drop();

	void Start();
	void End();
	void Pause();
	void Resume();
	void Restart();

	bool SaveGame(TCHAR* szArchive);
	bool LoadGame(TCHAR* szArchive);

public:
	GameFrame* GetGameFrame();
	TetrisShape* GetTetrisShape();
	TetrisShape* GetNextTetrisShape();
	Mass* GetMass();

private:
	const UINT_PTR ST_STEPDOWN = 1;
	const UINT_PTR ST_DROPDELAY = 2;
	const UINT_PTR ST_REMOVEBLINK = 3;
	const UINT_PTR ST_ROLL = 5;

	int removeBlinkTimes;
	int removeBlinkCount;
	
	bool StartStepDown(bool isDropping);
	bool EndStepDown();
	static void CALLBACK StepDownTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void StepDownTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartDropDelay();
	bool EndDropDelay();
	static void CALLBACK DropDelayTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void DropDelayTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartRemoveBlink();
	bool EndRemoveBlink();
	static void CALLBACK RemoveBlinkTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void RemoveBlinkTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

	bool StartRoll();
	bool EndRoll();
	static void CALLBACK RollTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void RollTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);

private:
	Controller() {};
	void InvalidateDraw();
	HWND hWnd;
	GameFrame* pGameFrame;
	Drawer* pDrawer;
	bool initialized;
	GameState gameState;

	UINT stepDownTimespan;
	UINT dropTimespan;
	UINT dropDelayTimespan;
	UINT removeDelayTimespan;
	UINT removeBlinkTimespan;
	UINT rollTimespan;
};

