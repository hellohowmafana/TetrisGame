#pragma once
#include <vector>
#include "GameFrame.h"
#include "PromptFrame.h"
#include "InfoFrame.h"
#include "Background.h"
#include <Unknwn.h>
#include <gdiplus.h>
using namespace std;
using namespace Gdiplus;

class Drawer
{
public:
	static Drawer singleton;

public:
	bool Initialize(GameFrame* pGameFrame, PromptFrame* pPromptFrame, InfoFrame* pInfoFrame, Background* pBackground);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();

private:
	Drawer();
	~Drawer();
	
	void DrawBackground();

	void DrawFrame(Frame* pFrame);
	void DrawBorder(UnitFrame* pUnitFrame);
	void DrawSeparators(UnitFrame* pUnitFrame);

	void DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape);
	void DrawMass(GameFrame* pGameFrame, Mass* pMass);
	void DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y);
	void DrawUnit(UnitFrame* pUnitFrame, int x, int y, HBRUSH brush);

	void DrawInfo(InfoFrame* pInfoFrame);

	void GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight);
	HBRUSH GetRandomTetrisBrush();
	bool IsValid();

private:
	GameFrame* pGameFrame;
	PromptFrame* pPromptFrame;
	InfoFrame* pInfoFrame;
	Background* pBackground;
	HDC hdc;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	vector<HBRUSH> vecTetrisBrushes;
	HBRUSH hbsMass;
	Bitmap* pbmBackground;
	COLORREF clBackground;
	HBRUSH hbsBackground;
	bool initialized;

	HDC hdcCmp;
	HBITMAP hbmCmp;
	LONG dcWidth, dcHeight;
};

