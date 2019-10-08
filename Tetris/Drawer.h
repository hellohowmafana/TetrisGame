#pragma once
#include <vector>
#include "Configuration.h"
#include "TetrisShape.h"
#include "Accretion.h"
using namespace std;

class Drawer
{
public:
	static Drawer singleton;

public:
	bool Initialize(Configuration* pConfiguration);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();

private:
	Drawer();

	void DrawFrame();
	void DrawBorder();
	void DrawSeparators();
	void DrawUnit(int x, int y, HBRUSH brush);
	void DrawShape();

	void DrawPromptFrame();
	void DrawPromptUnit(int x, int y, HBRUSH brush);
	void DrawPromptShape();

	void DrawInfoFrameFrame();

	void DrawBackgroud();

	void DrawTetrisShape(TetrisShape* pTetrisShape);
	void DrawAccretion(Accretion* pAccretion);

	~Drawer();

private:
	Configuration* pConfiguration;
	HDC hdc;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	vector<HBRUSH> vecTetrisBrushes;
	HBRUSH hbsAccretion;

	bool initialized;
};

