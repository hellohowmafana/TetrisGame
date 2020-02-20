#pragma once
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

#include <vector>
#include <Unknwn.h>
#include <gdiplus.h>
#include "AnimatedGifPoller.hpp"
#include "Configurable.hpp"
#include "GameFrame.hpp"

using namespace std;
using namespace Gdiplus;

class Controller;

class Drawer : public Configurable
{
public:
	static Drawer singleton;

public:
	bool OnUpdate(Configuration* pConfiguration);
	bool OnDeinitialize();
	void SetController(Controller* pController);
	void SetGraphics(GameFrame* pGameFrame, PromptFrame* pPromptFrame,
		InfoFrame* pInfoFrame, Background* pBackground);
	void SetHWnd(HWND hWnd);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();
	void Invalidate();

private:
	Drawer();
	~Drawer();
	
	void DrawSolidColor(const Color& color, const Rect& rect);
	void DrawBitmap(Bitmap* pBitmap, RenderMode renderMode,
		RenderAlignmentHorizontal renderAlignmentHorizontal,
		RenderAlignmentVertical renderAlignmentVertical, const Rect& rect);
	void DrawBackground();

	void DrawFrame(Frame* pFrame);
	void DrawBackground(Frame* pFrame);
	void DrawBorder(UnitFrame* pUnitFrame);
	void DrawSeparators(UnitFrame* pUnitFrame);

	Rect* GetUnitRectangle(UnitFrame* pUnitFrame, int x, int y, Rect* pRect);
	void DrawUnit(UnitFrame* pUnitFrame, int x, int y, Brush* pBrush);
	void DrawUnit(UnitFrame* pUnitFrame, int x, int y, Bitmap* pBitmap);
	void DrawLine(UnitFrame* pUnitFrame, int y, Brush* pBrush);
	void DrawLine(UnitFrame* pUnitFrame, int y, Bitmap* pBitmap);
	void DrawUnits(UnitFrame* pUnitFrame, float blankRate, Brush* pBrush);
	void DrawUnits(UnitFrame* pUnitFrame, float blankRate, Bitmap* pBitmap);
	void DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape, bool isLighting);
	void DrawMass(GameFrame* pGameFrame, Mass* pMass);
	void DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y);
	void DrawFill(UnitFrame* pUnitFrame, float blankRate);
	void DrawRollingLines(GameFrame* pGameFrame);

	void DrawInfo(InfoFrame* pInfoFrame);

	void DrawIcon(GameFrame* pGameFrame);
	void DrawMask(GameFrame* pGameFrame);
	void DrawSplash(GameFrame* pGameFrame);

	void GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight);
	void GetDCResolution(HDC hdc, int* px, int* py);
	void SetBitmapDCResolution(Bitmap* pBitmap, HDC hdc);
	Color LightColor(Color& color, float ratio);
	Bitmap* StretchBitmap(Bitmap* pBmp, int dstWidth, int dstHeight);
	Bitmap* LightBitmap(Bitmap* pBmp, float ratio);
	Bitmap* TranslateBitmap(Bitmap* pBmp, int offsetX, int offsetY);

	Brush* GetRandomTetrisBrush();
	
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

	Brush* pbrsFrame;
	Pen* ppenBorder;
	Pen* ppenSeprator;
	Bitmap* pbmpUnit;
	Bitmap* pbmpUnitLight;
	vector<Brush*> vecTetrisBrushes;
	vector<Brush*> vecTetrisBrushesLight;
	Brush* pbrsMass;
	Brush* pbrsMassLight;

	Bitmap* pbmpBackground;
	AnimatedGifPoller* pGifPollerBackground;
	bool isAnimatedBackground;

	Font* pfntInfo;
	Brush* pbrsInfo;

	Bitmap* pbmpBegin;
	Bitmap* pbmpGameOver;
	Bitmap* pbmpPause;
	Bitmap* pbmpResume;
	Brush* pbrsMask;
	
	bool attached;

	HDC hdcCmp;
	HBITMAP hbmCmp;
	LONG dcWidth, dcHeight;
};

