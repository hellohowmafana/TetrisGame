#include "Drawer.h"
#include "Utility.h"

#define CreateSolidPen(width, color) CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::singleton;

Drawer::Drawer() :
	pGameFrame(nullptr), hpnBorder(NULL), hpnSeparator(NULL), hbsMass(NULL),
	hdc(NULL), hdcCmp(NULL), hbmCmp(NULL), initialized(false),
	dcWidth(0), dcHeight(0)
{

}

Drawer::~Drawer()
{
	if (attached)
	{
		DetachDC();
	}

	if (initialized)
	{
		Deinitialize();
	}
}

bool Drawer::Initialize(GameFrame* pGameFrame, PromptFrame* pPromptFrame, InfoFrame*pInfoFrame, Background* pBackground)
{
	try {
		this->pGameFrame = pGameFrame;
		this->pPromptFrame = pPromptFrame;
		this->pInfoFrame = pInfoFrame;
		this->pBackground = pBackground;

		hpnBorder = CreateSolidPen(pGameFrame->borderThickness, *pGameFrame->pBorderColor);
		hpnSeparator = CreateSolidPen(pGameFrame->separatorThickness, *pGameFrame->pSeparatorColor);

		for (size_t i = 0; i < pGameFrame->pTetrisColors->size(); i++)
		{
			vecTetrisBrushes.push_back(CreateSolidBrush(pGameFrame->pTetrisColors->at(i)));
		}

		hbsMass = CreateSolidBrush(*pGameFrame->pMassColor);
		hbsMassLight = CreateSolidBrush(Utility::LightColor(*pGameFrame->pMassColor, 0.5));

		pBitmapBackground = new Bitmap(pBackground->pathBackground.c_str());
		pBitmapBackground->GetHBITMAP(Color(), &hbmBackground);
		hbsBackground = BackgroundMode::Tile == pBackground->backgroundMode ?
			CreatePatternBrush(hbmBackground) :
			CreateSolidBrush(pBackground->colorBackground);
		hftInfo = CreateFont(pInfoFrame->fontHeight, pInfoFrame->fontWidth, 0, 0,
			pInfoFrame->fontWeight*100, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, pInfoFrame->fontFace.c_str());

		HDC hdc = GetDC(NULL);
		pBitmapBackground->SetResolution((REAL)GetDeviceCaps(hdc, LOGPIXELSX),
			(REAL)GetDeviceCaps(hdc, LOGPIXELSY));
		DeleteDC(hdc);
	}
	catch (...){
		return false;
	}

	initialized = true;

	return true;
}

bool Drawer::Deinitialize()
{
	this->pGameFrame = nullptr;
	this->pPromptFrame = nullptr;
	this->pInfoFrame = nullptr;
	this->pBackground = nullptr;

	DeleteObject(hpnBorder);
	hpnBorder = NULL;
	DeleteObject(hpnSeparator);
	hpnSeparator = NULL;
	for (size_t i = 0; i < vecTetrisBrushes.size(); i++)
	{
		DeleteObject(vecTetrisBrushes.at(i));
	}
	vecTetrisBrushes.clear();
	DeleteObject(hbsMass);
	hbsMass = NULL;
	//delete pbmBackground;
	DeleteObject(hbmBackground);
	hbmBackground = NULL;
	DeleteObject(hbsBackground);
	hbsBackground = NULL;
	DeleteObject(hftInfo);
	hftInfo = NULL;

	initialized = false;

	return true;
}

void Drawer::SetHWnd(HWND hWnd)
{
	this->hWnd = hWnd;
}

void Drawer::AttachDC(HDC hdc)
{
	this->hdc = hdc;
	hdcCmp = CreateCompatibleDC(hdc);
	GetDCSize(hdc, &dcWidth, &dcHeight);
	hbmCmp = CreateCompatibleBitmap(hdc, dcWidth, dcHeight);
	SelectObject(hdcCmp, hbmCmp);
	attached = true;
}

void Drawer::DetachDC()
{
	this->hdc = NULL;
	dcWidth = dcHeight = 0;
	DeleteObject(hbmCmp);
	hbmCmp = NULL;
	DeleteDC(hdcCmp);
	hdcCmp = NULL;
	attached = false;
}

void Drawer::DrawElements()
{
	if (IsValid())
	{
		DrawBackground();
		DrawFrame(pGameFrame);
		DrawShape(pGameFrame, pGameFrame->GetShape());
		DrawMass(pGameFrame, pGameFrame->GetMass());
		DrawFrame(pPromptFrame);
		DrawShape(pPromptFrame, pPromptFrame->GetTerisShape());
		DrawFrame(pInfoFrame);
		DrawInfo(pInfoFrame);

		BitBlt(hdc, 0, 0, dcWidth, dcHeight, hdcCmp, 0, 0, SRCCOPY);
	}
}

void Drawer::Invalid()
{
	InvalidateRgn(hWnd, NULL, FALSE);
}

void Drawer::DrawBackground()
{
	if (!IsValid()) return;

	switch (pBackground->backgroundMode)
	{
		case BackgroundMode::Color:
		{
			HGDIOBJ hbsOld = SelectObject(hdcCmp, hbsBackground);
			ExtFloodFill(hdcCmp, 0, 0, RGB(0, 0, 0), FLOODFILLSURFACE);
			SelectObject(hdcCmp, hbsOld);
		}
		break;
		case BackgroundMode::General:
		{
			Graphics graphics(hdcCmp);
			graphics.DrawImage(pBitmapBackground, 0, 0);
		}
		break;
		case BackgroundMode::Strech:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			Graphics graphics(hdcCmp);
			graphics.DrawImage(pBitmapBackground, 0, 0, rect.right - rect.left, rect.bottom - rect.top);
		}
		break;
		case BackgroundMode::Tile:
		{
			SelectObject(hdcCmp, hbsBackground);
			ExtFloodFill(hdcCmp, 0, 0, RGB(0, 0, 0), FLOODFILLSURFACE);
		}
		break;
		case BackgroundMode::UniformFill:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int bitmapWidth = pBitmapBackground->GetWidth();
			int bitmapHeight = pBitmapBackground->GetHeight();
			int clientWidth = rect.right - rect.left;
			int clientHeight = rect.bottom - rect.top;

			REAL rateWidth = (REAL)bitmapWidth / clientWidth;
			REAL rateHeight = (REAL)bitmapHeight / clientHeight;
			REAL rate = rateWidth < rateHeight ? rateWidth : rateHeight;

			bool fitWidth = rate == rateWidth;
			REAL srcWidth = fitWidth ? bitmapWidth : clientWidth * rate;
			REAL srcHeight = !fitWidth ? bitmapHeight : clientHeight * rate;
			REAL srcX = fitWidth ? 0 : (bitmapWidth / rate - clientWidth) / 2;
			REAL srcY = !fitWidth ? 0 : (bitmapHeight / rate - clientHeight) / 2;

			Graphics graphics(hdcCmp);
			graphics.DrawImage(pBitmapBackground, RectF(0, 0, (REAL)clientWidth, (REAL)clientHeight),
				srcX, srcY, srcWidth, srcHeight, Unit::UnitPixel);
		}
		break;
		case BackgroundMode::Uniform:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			int bitmapWidth = pBitmapBackground->GetWidth();
			int bitmapHeight = pBitmapBackground->GetHeight();
			int clientWidth = rect.right - rect.left;
			int clientHeight = rect.bottom - rect.top;

			REAL rateWidth = (REAL)bitmapWidth / clientWidth;
			REAL rateHeight = (REAL)bitmapHeight / clientHeight;
			REAL rate = rateWidth > rateHeight ? rateWidth : rateHeight;

			bool fitWidth = rate == rateWidth;
			REAL width = fitWidth ? clientWidth : (bitmapWidth / rate);
			REAL height = !fitWidth ? clientHeight : (bitmapHeight / rate);
			REAL x = fitWidth ? 0 : (clientWidth - width) / 2;
			REAL y = !fitWidth ? 0 : (clientHeight - height) / 2;

			SelectObject(hdcCmp, hbsBackground);
			ExtFloodFill(hdcCmp, 0, 0, RGB(0, 0, 0), FLOODFILLSURFACE);
			Graphics graphics(hdcCmp);
			graphics.DrawImage(pBitmapBackground, x, y, width, height);
		}
		break;
	}
}

void Drawer::DrawFrame(Frame* pFrame)
{
	if (!IsValid()) return;

	if (dynamic_cast<UnitFrame*>(pFrame) != nullptr)
	{
		UnitFrame* pUnitFrame = (UnitFrame*)pFrame;
		DrawBorder(pUnitFrame);
		DrawSeparators(pUnitFrame);
	}
	if (dynamic_cast<InfoFrame*>(pFrame) != nullptr)
	{
	}
}

void Drawer::DrawBorder(UnitFrame* pUnitFrame)
{
	if (!IsValid()) return;

	if (pUnitFrame->borderThickness == 0) return;

	SelectObject(hdcCmp, hpnBorder);
	SelectObject(hdcCmp, GetStockObject(WHITE_BRUSH));
	int innerWidth = (pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeX
		+ pUnitFrame->separatorThickness;
	int innerHeight = (pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeY
		+ pUnitFrame->separatorThickness;
	int totalWidth = pUnitFrame->borderThickness * 2 + innerWidth;
	int totalHeight = pUnitFrame->borderThickness * 2 + innerHeight;
	int left = pUnitFrame->left + pUnitFrame->borderThickness / 2;
	int top = pUnitFrame->top + pUnitFrame->borderThickness / 2;
	int width = totalWidth - pUnitFrame->borderThickness;
	int height = totalHeight - pUnitFrame->borderThickness;
	Rectangle(hdcCmp, left, top, left + width + 1, top + height + 1);
}

void Drawer::DrawSeparators(UnitFrame* pUnitFrame)
{
	if (!IsValid()) return;

	if (pUnitFrame->separatorThickness == 0) return;

	SelectObject(hdcCmp, hpnSeparator);

	int top = pUnitFrame->top + pUnitFrame->borderThickness;
	int bottom = top +
		(pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeY
		+ pUnitFrame->separatorThickness;

	for (int n = 0; n <= pUnitFrame->sizeX; n++)
	{
		int x1 = pUnitFrame->left + pUnitFrame->borderThickness
			+ n * (pUnitFrame->unitWidth + pUnitFrame->separatorThickness)
			+ pUnitFrame->separatorThickness / 2;
		int x2 = x1 + 1;
		Rectangle(hdcCmp, x1, top, x2, bottom + 1);
	}

	int left = pUnitFrame->left + pUnitFrame->borderThickness;
	int right = left +
		(pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeX
		+ pUnitFrame->separatorThickness;

	for (int n = 0; n <= pUnitFrame->sizeY; n++)
	{
		int y1 = pUnitFrame->top + pUnitFrame->borderThickness
			+ n * (pUnitFrame->unitWidth + pUnitFrame->separatorThickness)
			+ pUnitFrame->separatorThickness / 2;
		int y2 = y1 + 1;
		Rectangle(hdcCmp, left, y1, right, y2);
	}
}

void Drawer::DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape)
{
	if (!IsValid()) return;

	if (nullptr == pTetrisShape) return;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if(pTetrisShape->IsSolid(i, j, true))
				DrawUnit(pUnitFrame, i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
		}
	}
}

void Drawer::DrawMass(GameFrame* pGameFrame, Mass* pMass)
{
	if (!IsValid()) return;

	if (nullptr == pMass) return;

	int y = pMass->GetTop();
	for (MassBlock::iterator it = pMass->GetTopLineIterator();
		it != pMass->GetBottomLineIterator(); it++)
	{
		DrawMassLine(pGameFrame, *it, y++);
	}
}

void Drawer::DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y)
{
	if (!IsValid()) return;

	int x = 0;
	for (MassLine::iterator it = pMassLine->begin(); it != pMassLine->end(); it++)
	{
		HBRUSH hbs;
		if (pGameFrame->IsBlinkingLight() && pGameFrame->IsBlinkingLine(pMassLine))
			hbs = hbsMassLight;
		else
			hbs = pGameFrame->useMassColor ? hbsMass : vecTetrisBrushes[it->color];
		if (it->isSolid)
		{
			DrawUnit(pGameFrame, x, y, hbs);
		}
		x++;
	}
}

void Drawer::DrawUnit(UnitFrame* pUnitFrame, int x, int y, HBRUSH brush)
{
	if (!IsValid()) return;

	if (x < 0 || x >= pUnitFrame->sizeX
		|| y < 0 || y >= pUnitFrame->sizeY)
		return;

	LONG left = pUnitFrame->left + pUnitFrame->borderThickness
		+ pUnitFrame->separatorThickness * (x + 1) + pUnitFrame->unitWidth * x;
	LONG right = left + pUnitFrame->unitWidth;
	LONG top = pUnitFrame->top + pUnitFrame->borderThickness
		+ pUnitFrame->separatorThickness * (y + 1) + pUnitFrame->unitWidth * y;
	LONG bottom = top + pUnitFrame->unitWidth;
	RECT rect = { left, top, right, bottom };
	FillRect(hdcCmp, &rect, brush);
}

void Drawer::DrawInfo(InfoFrame* pInfoFrame)
{
	tstring labels;
	labels.append(_T("Level\n\n"));
	labels.append(_T("Score\n\n"));
	labels.append(_T("StartLine\n\n"));
	tstring infos;
	infos.append(to_tstring(*pInfoFrame->pLevel)).append(_T("\n\n"));
	infos.append(to_tstring(*pInfoFrame->pScore)).append(_T("\n\n"));
	infos.append(to_tstring(*pInfoFrame->pStartLine));
	RECT rcInfo = { pInfoFrame->left, pInfoFrame->top,
		pInfoFrame->left + pInfoFrame->sizeX, pInfoFrame->top + pInfoFrame->sizeY };
	SetBkMode(hdcCmp, TRANSPARENT);
	SetTextColor(hdcCmp, pInfoFrame->colorInfo);
	SelectObject(hdcCmp, hftInfo);
	DrawText(hdcCmp, labels.c_str(), labels.size(), &rcInfo, DT_LEFT);
	DrawText(hdcCmp, infos.c_str(), infos.size(), &rcInfo, DT_RIGHT);
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

HBRUSH Drawer::GetRandomTetrisBrush()
{
	return vecTetrisBrushes[Utility::Random(0, vecTetrisBrushes.size() - 1)];
}

bool Drawer::IsValid()
{
	return initialized && hdc && hdcCmp;
}
