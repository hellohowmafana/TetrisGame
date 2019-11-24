#pragma once
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

#include <vector>
#include "GameFrame.h"
#include "PromptFrame.h"
#include "InfoFrame.h"
#include "Background.h"
#include <Unknwn.h>
#include <gdiplus.h>
#include "AnimatedGifPoller.h"

using namespace std;
using namespace Gdiplus;

class Controller;

class Drawer
{
public:
	static Drawer singleton;

public:
	bool Initialize(Controller* pController, GameFrame* pGameFrame,
		PromptFrame* pPromptFrame, InfoFrame* pInfoFrame, Background* pBackground);
	bool Deinitialize();
	void SetHWnd(HWND hWnd);
	void AttachDC(HDC hdc);
	void DetachDC();
	bool IsInitialized();
	void DrawElements();
	void Invalidate();

private:
	Drawer();
	~Drawer();
	
	void DrawBitmap(HBITMAP hbmBitmap, RenderMode renderMode, RenderAlignment renderAlignment, RECT* pRect);
	void DrawBackground();

	void DrawFrame(Frame* pFrame);
	void DrawBorder(UnitFrame* pUnitFrame);
	void DrawSeparators(UnitFrame* pUnitFrame);

	void DrawUnit(UnitFrame* pUnitFrame, int x, int y, HGDIOBJ gdiObj, bool isBitmap);
	void DrawLine(UnitFrame* pUnitFrame, int y, HGDIOBJ gdiObj, bool isBitmap);
	void DrawUnits(UnitFrame* pUnitFrame, double blankRate, bool leanBlank, HGDIOBJ gdiObj, bool isBitmap);
	void DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape);
	void DrawMass(GameFrame* pGameFrame, Mass* pMass);
	void DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y);
	void DrawFill(UnitFrame* pUnitFrame, double blankRate);
	void DrawRollingLines(GameFrame* pGameFrame);

	void DrawInfo(InfoFrame* pInfoFrame);

	void DrawIcon(GameFrame* pGameFrame);
	void DrawMask(GameFrame* pGameFrame);
	void DrawSplash(GameFrame* pGameFrame);

	void GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight);
	void GetDCResolution(HDC hdc, int* px, int* py);
	void SetBitmapDCResolution(Bitmap* pBitmap, HDC hdc);
	COLORREF LightColor(COLORREF color, double ratio);
	HBITMAP StretchBitmap(HDC hdcRef, HBITMAP hbm, int dstWidth, int dstHeight);
	HBITMAP LightBitmap(HDC hdcRef, HBITMAP hbm, double ratio);
	HBITMAP TranslateBitmap(HDC hdcRef, HBITMAP hbm, int offsetX, int offsetY);
	HBITMAP CreateHBITMAP(Bitmap* pBitmap);

	HBRUSH GetRandomTetrisBrush();
	
	bool IsValid();

	static void CALLBACK BackgroundFrameChangedProcStatic(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame);
	void BackgroundFrameChangedProc(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame);

private:
	Controller* pController;
	GameFrame* pGameFrame;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;
	Background* pBackground;
	HDC hdc;
	HWND hWnd;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	HBITMAP hbmUnit;
	HBITMAP hbmUnitLight;
	vector<HBRUSH> vecTetrisBrushes;
	HBRUSH hbsMass;
	HBRUSH hbsMassLight;
	// background
	Bitmap* pBitmapBackground;
	HBITMAP hbmBackground;
	AnimatedGifPoller* pAnimatedGifPoller;
	bool isAnimatedBackground;
	HFONT hftInfo;
	Bitmap* pBitmapBegin;
	Bitmap* pBitmapGameOver;
	Bitmap* pBitmapPause;
	Bitmap* pBitmapResume;
	Brush* pBrushMask;
	
	bool initialized;
	bool attached;

	HDC hdcCmp;
	HBITMAP hbmCmp;
	LONG dcWidth, dcHeight;
};

