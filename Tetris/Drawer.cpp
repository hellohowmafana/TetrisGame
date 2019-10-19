#include "Drawer.h"

#define CreateSolidPen(width, color) CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::singleton;

Drawer::Drawer() :
	pGameFrame(nullptr), hpnBorder(NULL), hpnSeparator(NULL), hbsMass(NULL),
	hdc(NULL), hdcCmp(NULL), hbmCmp(NULL), initialized(false),
	dcWidth(0), dcHeight(0)
{

}

bool Drawer::Initialize(GameFrame* pGameFrame)
{
	try {
		this->pGameFrame = pGameFrame;

		hpnBorder = CreateSolidPen(pGameFrame->borderThickness, *pGameFrame->pBorderColor);
		hpnSeparator = CreateSolidPen(pGameFrame->separatorThickness, *pGameFrame->pSeparatorColor);

		for (size_t i = 0; i < pGameFrame->pTetrisColors->size(); i++)
		{
			vecTetrisBrushes.push_back(CreateSolidBrush(pGameFrame->pTetrisColors->at(i)));
		}

		hbsMass = CreateSolidBrush(*pGameFrame->pMassColor);
	}
	catch (...){
		return false;
	}

	initialized = true;

	return true;
}

void Drawer::AttachDC(HDC hdc)
{
	this->hdc = hdc;
	hdcCmp = CreateCompatibleDC(hdc);
	GetDCSize(hdc, &dcWidth, &dcHeight);
	hbmCmp = CreateCompatibleBitmap(hdc, dcWidth, dcHeight);
	SelectObject(hdcCmp, hbmCmp);
}

void Drawer::DetachDC()
{
	this->hdc = NULL;
	dcWidth = dcHeight = 0;
	DeleteObject(hbmCmp);
	hbmCmp = NULL;
	DeleteDC(hdcCmp);
	hdcCmp = NULL;
}

void Drawer::DrawElements()
{
	if (initialized && hdcCmp && hdc)
	{
		DrawFrame();
		DrawShape(pGameFrame->GetShape());
		DrawMass(pGameFrame->GetMass());
		BitBlt(hdc, 0, 0, dcWidth, dcHeight, hdcCmp, 0, 0, SRCCOPY);
	}
}

void Drawer::DrawFrame()
{
	if (!initialized || NULL == hdcCmp) return;

	DrawBorder();
	DrawSeparators();
}

void Drawer::DrawBorder()
{
	if (!initialized || NULL == hdcCmp) return;

	if (pGameFrame->borderThickness == 0) return;

	SelectObject(hdcCmp, hpnBorder);
	int innerWidth = (pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeX
		+ pGameFrame->separatorThickness;
	int innerHeight = (pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeY
		+ pGameFrame->separatorThickness;
	int totalWidth = pGameFrame->borderThickness * 2 + innerWidth;
	int totalHeight = pGameFrame->borderThickness * 2 + innerHeight;
	int left = pGameFrame->left + pGameFrame->borderThickness / 2;
	int top = pGameFrame->top + pGameFrame->borderThickness / 2;
	int width = totalWidth - pGameFrame->borderThickness;
	int height = totalHeight - pGameFrame->borderThickness;
	Rectangle(hdcCmp, left, top, left + width + 1, top + height + 1);
}

void Drawer::DrawSeparators()
{
	if (!initialized || NULL == hdcCmp) return;

	if (pGameFrame->separatorThickness == 0) return;

	SelectObject(hdcCmp, hpnSeparator);

	int top = pGameFrame->top + pGameFrame->borderThickness;
	int bottom = top +
		(pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeY
		+ pGameFrame->separatorThickness;

	for (int n = 0; n <= pGameFrame->sizeX; n++)
	{
		int x1 = pGameFrame->left + pGameFrame->borderThickness
			+ n * (pGameFrame->unitWidth + pGameFrame->separatorThickness)
			+ pGameFrame->separatorThickness / 2;
		int x2 = x1 + 1;
		Rectangle(hdcCmp, x1, top, x2, bottom + 1);
	}

	int left = pGameFrame->left + pGameFrame->borderThickness;
	int right = left +
		(pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeX
		+ pGameFrame->separatorThickness;

	for (int n = 0; n <= pGameFrame->sizeY; n++)
	{
		int y1 = pGameFrame->top + pGameFrame->borderThickness
			+ n * (pGameFrame->unitWidth + pGameFrame->separatorThickness)
			+ pGameFrame->separatorThickness / 2;
		int y2 = y1 + 1;
		Rectangle(hdcCmp, left, y1, right, y2);
	}
}

void Drawer::DrawBackgroud()
{
	if (!initialized || NULL == hdcCmp) return;
}

void Drawer::DrawShape(TetrisShape* pTetrisShape)
{
	if (!initialized || NULL == hdcCmp) return;

	if (nullptr == pTetrisShape) return;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if(pTetrisShape->IsSolid(i, j, true))
				DrawUnit(i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
		}
	}
}

void Drawer::DrawMass(Mass* pMass)
{
	if (!initialized || NULL == hdcCmp) return;

	if (nullptr == pMass) return;

	int y = pMass->GetTop();
	for (MassBlock::iterator it = pMass->GetTopLineIterator();
		it != pMass->GetBottomLineIterator(); it++)
	{
		DrawMassLine(*it, y++);
	}
}

void Drawer::DrawMassLine(MassLine* pMassLine, int y)
{
	if (!initialized || NULL == hdcCmp) return;

	int x = 0;
	for (MassLine::iterator it = pMassLine->begin(); it != pMassLine->end(); it++)
	{
		if (it->isSolid)
		{
			DrawUnit(x, y, pGameFrame->useMassColor ? hbsMass : vecTetrisBrushes[it->color]);
		}
		x++;
	}
}

void Drawer::DrawUnit(int x, int y, HBRUSH brush)
{
	if (!initialized || NULL == hdcCmp) return;

	if (x < 0 || x >= pGameFrame->sizeX
		|| y < 0 || y >= pGameFrame->sizeY)
		return;

	LONG left = pGameFrame->left + pGameFrame->borderThickness
		+ pGameFrame->separatorThickness * (x + 1) + pGameFrame->unitWidth * x;
	LONG right = left + pGameFrame->unitWidth;
	LONG top = pGameFrame->top + pGameFrame->borderThickness
		+ pGameFrame->separatorThickness * (y + 1) + pGameFrame->unitWidth * y;
	LONG bottom = top + pGameFrame->unitWidth;
	RECT rect = { left, top, right, bottom };
	FillRect(hdcCmp, &rect, brush);
}

void Drawer::DrawPromptFrame()
{
}

void Drawer::DrawPromptUnit(int x, int y, HBRUSH brush)
{
}

void Drawer::DrawPromptShape()
{
}

void Drawer::DrawInfoFrameFrame()
{
}

Drawer::~Drawer()
{
	DeleteObject(hpnBorder);
	hpnBorder = NULL;
	DeleteObject(hpnSeparator);
	hpnSeparator = NULL;
	if (NULL != hbmCmp)
	{
		DeleteObject(hbmCmp);
		hbmCmp = NULL;
	}
	if (NULL != hdcCmp)
	{
		DeleteDC(hdcCmp);
		hdcCmp = NULL;
	}
}

void Drawer::GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight)
{
	BITMAP bm;
	memset(&bm, 0, sizeof(BITMAP));
	HGDIOBJ hbm = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hbm, sizeof(BITMAP), &bm);
	*pWidth = bm.bmWidth;
	*pHeight = bm.bmHeight;
}
