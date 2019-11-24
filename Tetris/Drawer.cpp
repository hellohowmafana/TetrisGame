#include "Drawer.h"
#include "Utility.h"
#include "Controller.h"
#include <bitset>
#define CreateSolidPen(width, color) CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::singleton;

Drawer::Drawer() :
	pGameFrame(nullptr), hpnBorder(NULL), hpnSeparator(NULL), hbsMass(NULL),
	hdc(NULL), hdcCmp(NULL), hbmCmp(NULL), initialized(false),
	dcWidth(0), dcHeight(0),
	pAnimatedGifPoller(nullptr)
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

bool Drawer::Initialize(Controller* pController, GameFrame* pGameFrame,
	PromptFrame* pPromptFrame, InfoFrame*pInfoFrame, Background* pBackground)
{
	if (NULL == hWnd)
		return false;
	try {
		this->pController = pController;
		this->pGameFrame = pGameFrame;
		this->pPromptFrame = pPromptFrame;
		this->pInfoFrame = pInfoFrame;
		this->pBackground = pBackground;

		HDC hdcRef = GetDC(hWnd);

		hpnBorder = CreateSolidPen(pGameFrame->borderThickness, *pGameFrame->pBorderColor);
		hpnSeparator = CreateSolidPen(pGameFrame->separatorThickness, *pGameFrame->pSeparatorColor);

		if (pGameFrame->useColor)
		{
			for (size_t i = 0; i < pGameFrame->pTetrisColors->size(); i++)
			{
				vecTetrisBrushes.push_back(CreateSolidBrush(pGameFrame->pTetrisColors->at(i)));
			}

			hbsMass = CreateSolidBrush(*pGameFrame->pMassColor);
			hbsMassLight = CreateSolidBrush(LightColor(*pGameFrame->pMassColor, 0.5));
		}
		else
		{
			Bitmap bitmapUnit(pGameFrame->pathUnitBitmap.c_str());
			HBITMAP hbmUnitBitmap = CreateHBITMAP(&bitmapUnit);
			hbmUnit = StretchBitmap(hdcRef, hbmUnitBitmap, pGameFrame->unitWidth, pGameFrame->unitWidth);
			hbmUnitLight = LightBitmap(hdcRef, hbmUnit, 0.5);
			DeleteObject(hbmUnitBitmap);
		}

		if (RenderMode::Color != pBackground->renderMode)
		{
			pBitmapBackground = new Bitmap(pBackground->pathBackground.c_str());
			if (Status::Ok != pBitmapBackground->GetLastStatus())
				return false;
			SetBitmapDCResolution(pBitmapBackground, hdcRef);
			hbmBackground = CreateHBITMAP(pBitmapBackground);
			if (AnimatedGifPoller::IsAnimatedGif(pBitmapBackground))
			{
				pAnimatedGifPoller = new AnimatedGifPoller(pBitmapBackground, BackgroundFrameChangedProcStatic);
				pAnimatedGifPoller->SetLoopInfinate(true);
				pAnimatedGifPoller->Start();
			}
			isAnimatedBackground = AnimatedGifPoller::IsAnimatedGif(pBitmapBackground);
		}

		hftInfo = CreateFont(pInfoFrame->fontHeight, pInfoFrame->fontWidth, 0, 0,
			pInfoFrame->fontWeight*100, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, pInfoFrame->fontFace.c_str());

		pBitmapBegin = new Bitmap(pGameFrame->pathBeginSplash.c_str());
		if (Status::Ok != pBitmapBegin->GetLastStatus())
			return false;

		pBitmapGameOver = new Bitmap(pGameFrame->pathGameOverSplash.c_str());
		if (Status::Ok != pBitmapGameOver->GetLastStatus())
			return false;

		pBitmapPause = new Bitmap(pGameFrame->pathPauseIcon.c_str());
		if (Status::Ok != pBitmapPause->GetLastStatus())
			return false;
		pBitmapResume = new Bitmap(pGameFrame->pathResumeIcon.c_str());
		if (Status::Ok != pBitmapResume->GetLastStatus())
			return false;

		pBrushMask = new SolidBrush(Color((BYTE)(255 * pGameFrame->maskTransparency), 255, 255, 255));

		ReleaseDC(hWnd, hdcRef);
	}
	catch (...){
		return false;
	}

	initialized = true;

	return true;
}

bool Drawer::Deinitialize()
{
	if (!initialized) return false;

	this->pGameFrame = nullptr;
	this->pPromptFrame = nullptr;
	this->pInfoFrame = nullptr;
	this->pBackground = nullptr;

	DeleteObject(hpnBorder);
	hpnBorder = NULL;
	DeleteObject(hpnSeparator);
	hpnSeparator = NULL;
	if (pGameFrame->useColor)
	{
		for (size_t i = 0; i < vecTetrisBrushes.size(); i++)
		{
			DeleteObject(vecTetrisBrushes.at(i));
		}
		vecTetrisBrushes.clear();
		DeleteObject(hbsMass);
		hbsMass = NULL;
	}
	else
	{
		DeleteObject(hbmUnit);
		hbmUnit = NULL;
		DeleteObject(hbmUnitLight);
		hbmUnitLight = NULL;
	}

	if (RenderMode::Color != pBackground->renderMode)
	{
		delete pBitmapBackground;
		pBitmapBackground = nullptr;
		DeleteObject(hbmBackground);
		hbmBackground = NULL;
		if (pAnimatedGifPoller)
		{
			delete pAnimatedGifPoller;
			pAnimatedGifPoller = nullptr;
		}
	}

	DeleteObject(hftInfo);
	hftInfo = NULL;

	delete pBitmapBegin;
	pBitmapBegin = nullptr;
	delete pBitmapGameOver;
	pBitmapGameOver = nullptr;

	delete pBitmapPause;
	pBitmapPause = nullptr;
	delete pBitmapResume;
	pBitmapResume = nullptr;

	delete pBrushMask;
	pBrushMask = nullptr;

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
	GetDCSize(hdc, &dcWidth, &dcHeight);
	
	hdcCmp = CreateCompatibleDC(hdc);
	SaveDC(hdcCmp);
	
	hbmCmp = CreateCompatibleBitmap(hdc, dcWidth, dcHeight);
	SelectObject(hdcCmp, hbmCmp);
	
	attached = true;
}

void Drawer::DetachDC()
{
	this->hdc = NULL;
	dcWidth = dcHeight = 0;
	
	RestoreDC(hdcCmp, 1);
	DeleteDC(hdcCmp);
	hdcCmp = NULL;

	DeleteObject(hbmCmp);
	hbmCmp = NULL;
	
	attached = false;
}

bool Drawer::IsInitialized()
{
	return initialized;
}

void Drawer::DrawElements()
{
	if (IsValid())
	{
		DrawBackground();
		DrawFrame(pGameFrame);
		DrawFrame(pPromptFrame);
		DrawFrame(pInfoFrame);
		DrawInfo(pInfoFrame);

		if (pController->IsStarted())
		{
			DrawShape(pGameFrame, pGameFrame->GetShape());
			DrawMass(pGameFrame, pGameFrame->GetMass());
			DrawMask(pGameFrame);
			DrawIcon(pGameFrame);
		
			DrawShape(pPromptFrame, pPromptFrame->GetTerisShape());
		}
		else
		{
			if(GameState::None == pController->GetGameState())
				DrawFill(pGameFrame, 0);
			else
				DrawRollingLines(pGameFrame);
			DrawSplash(pGameFrame);
		}

		BitBlt(hdc, 0, 0, dcWidth, dcHeight, hdcCmp, 0, 0, SRCCOPY);
	}
}

void Drawer::Invalidate()
{
	InvalidateRgn(hWnd, NULL, FALSE);
}

void Drawer::DrawBitmap(HBITMAP hbmBitmap, RenderMode renderMode, RenderAlignment renderAlignment, RECT* pRect)
{
	if (!IsValid()) return;

	if (RenderMode::Color == renderMode)
		return;

	switch (renderMode)
	{
	case RenderMode::General:
	{
		Graphics graphics(hdcCmp);
		Bitmap bitmap(hbmBitmap, NULL);
		graphics.DrawImage(&bitmap, pRect->left, pRect->top);
		break;
	}
	case RenderMode::Strech:
	{
		Graphics graphics(hdcCmp);
		Bitmap bitmap(hbmBitmap, NULL);
		graphics.DrawImage(&bitmap, pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top);
		break;
	}
	case RenderMode::Tile:
	{
		HBRUSH hbs = CreatePatternBrush(hbmBitmap);
		FillRect(hdcCmp, pRect, hbs);
		DeleteObject(hbs);
		break;
	}
	case RenderMode::UniformFill:
	{
		Bitmap bitmap(hbmBitmap, NULL);

		int bitmapWidth = bitmap.GetWidth();
		int bitmapHeight = bitmap.GetHeight();
		int clientWidth = pRect->right - pRect->left;
		int clientHeight = pRect->bottom - pRect->top;

		REAL rateWidth = (REAL)bitmapWidth / clientWidth;
		REAL rateHeight = (REAL)bitmapHeight / clientHeight;
		REAL rate = rateWidth < rateHeight ? rateWidth : rateHeight;

		bool fitWidth = rate == rateWidth;
		REAL srcWidth = fitWidth ? bitmapWidth : clientWidth * rate;
		REAL srcHeight = !fitWidth ? bitmapHeight : clientHeight * rate;
		REAL srcX = fitWidth ? 0 : (bitmapWidth / rate - clientWidth) / 2;
		REAL srcY = !fitWidth ? 0 : (bitmapHeight / rate - clientHeight) / 2;

		Graphics graphics(hdcCmp);
		graphics.DrawImage(&bitmap,
			RectF((REAL)pRect->left, (REAL)pRect->top, (REAL)clientWidth, (REAL)clientHeight),
			srcX, srcY, srcWidth, srcHeight, Unit::UnitPixel);

		break;
	}
	case RenderMode::Uniform:
	{
		Bitmap bitmap(hbmBitmap, NULL);

		int bitmapWidth = bitmap.GetWidth();
		int bitmapHeight = bitmap.GetHeight();
		int clientWidth = pRect->right - pRect->left;
		int clientHeight = pRect->bottom - pRect->top;

		REAL rateWidth = (REAL)bitmapWidth / clientWidth;
		REAL rateHeight = (REAL)bitmapHeight / clientHeight;
		REAL rate = rateWidth > rateHeight ? rateWidth : rateHeight;

		bool fitWidth = rate == rateWidth;
		REAL width = fitWidth ? clientWidth : (bitmapWidth / rate);
		REAL height = !fitWidth ? clientHeight : (bitmapHeight / rate);
		REAL x = (fitWidth ? 0 : (clientWidth - width) / 2) + pRect->left;
		REAL y = (!fitWidth ? 0 : (clientHeight - height) / 2) + pRect->top;

		Graphics graphics(hdcCmp);
		graphics.DrawImage(&bitmap, x, y, width, height);

		break;
	}
	}
}

void Drawer::DrawBackground()
{
	if (!IsValid()) return;

	// cover background with solid color
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBRUSH hbsBackgroundColor = CreateSolidBrush(pBackground->colorBackground);
	FillRect(hdcCmp, &rect, hbsBackgroundColor);
	DeleteObject(hbsBackgroundColor);

	// draw bitmap with mode
	if(RenderMode::Color != pBackground->renderMode)
		DrawBitmap(hbmBackground, pBackground->renderMode,
			pBackground->renderAlignment, &rect);
}

void Drawer::DrawFrame(Frame* pFrame)
{
	if (!IsValid()) return;

	if (dynamic_cast<const UnitFrame*>(pFrame) != nullptr)
	{
		UnitFrame* pUnitFrame = (UnitFrame*)pFrame;
		DrawBorder(pUnitFrame);
		DrawSeparators(pUnitFrame);
	}
	if (dynamic_cast<const InfoFrame*>(pFrame) != nullptr)
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

void Drawer::DrawUnit(UnitFrame* pUnitFrame, int x, int y, HGDIOBJ gdiObj, bool isBitmap)
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
	if (isBitmap)
	{
		Bitmap bitmap((HBITMAP)gdiObj, NULL);
		Graphics graphics(hdcCmp);
		graphics.DrawImage(&bitmap, rect.left, rect.top);
	}
	else
	{
		FillRect(hdcCmp, &rect, (HBRUSH)gdiObj);
	}
}

void Drawer::DrawLine(UnitFrame* pUnitFrame, int y, HGDIOBJ gdiObj, bool isBitmap)
{
	for (int i = 0; i < pUnitFrame->sizeX; i++)
	{
		DrawUnit(pUnitFrame, i, y, gdiObj, isBitmap);
	}
}

void Drawer::DrawUnits(UnitFrame* pUnitFrame, double blankRate, bool leanBlank, HGDIOBJ gdiObj, bool isBitmap)
{
	if (1 == blankRate)
		return;

	if (0 == blankRate)
	{
		for (int i = 0; i < pUnitFrame->GetWidth(); i++)
		{
			for (int j = 0; j < pUnitFrame->GetHeight(); j++)
			{
				DrawUnit(pUnitFrame, i, j, gdiObj, isBitmap);
			}
		}
	}
	else
	{
		int count = pUnitFrame->GetWidth() * pUnitFrame->GetHeight();
		int blankCount;
		if (leanBlank)
			blankCount = (int)floor(count * blankRate);
		else
			blankCount = (int)ceil(count * blankRate);
		vector<bool> vecSolids(count, false);
		for (size_t i = 0; i < (size_t)(count - blankCount); i++)
		{
			vecSolids[i] = true;
		}
		random_shuffle(vecSolids.begin(), vecSolids.end());

		for (int i = 0; i < count; i++)
		{
			if (vecSolids[i])
			{
				int x = i % pUnitFrame->GetWidth();
				int y = i / pUnitFrame->GetWidth();
				DrawUnit(pUnitFrame, x, y, gdiObj, isBitmap);
			}
		}
	}
}

void Drawer::DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape)
{
	if (GameState::None == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
		return;

	if (!IsValid()) return;

	if (nullptr == pTetrisShape) return;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if(pTetrisShape->IsSolid(i, j, true))
				DrawUnit(pUnitFrame, i, j,
					pGameFrame->useColor ? (HGDIOBJ)vecTetrisBrushes[pTetrisShape->GetColor()]: (HGDIOBJ)hbmUnit,
					!pGameFrame->useColor);
		}
	}
}

void Drawer::DrawMass(GameFrame* pGameFrame, Mass* pMass)
{
	if (GameState::None == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
		return;

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
	for (MassLine::const_iterator it = pMassLine->begin(); it != pMassLine->end(); it++)
	{
		if (it->isSolid)
		{
			bool useColor = pGameFrame->useColor;
			if (useColor)
			{
				HBRUSH hbs;
				if (GameState::BlinkLight == pController->GetGameState() && pGameFrame->IsLastFullLine(pMassLine))
					hbs = hbsMassLight;
				else
					hbs = pGameFrame->useMassColor ? hbsMass : vecTetrisBrushes[it->color];
				DrawUnit(pGameFrame, x, y, (HGDIOBJ)hbs, false);
			}
			else
			{
				HBITMAP hbm;
				if (GameState::BlinkLight == pController->GetGameState() && pGameFrame->IsLastFullLine(pMassLine))
					hbm = hbmUnitLight;
				else
					hbm = hbmUnit;
				DrawUnit(pGameFrame, x, y, (HGDIOBJ)hbm, true);
			}
		}
		x++;
	}
}

void Drawer::DrawFill(UnitFrame* pUnitFrame, double blankRate)
{
	DrawUnits(pUnitFrame, blankRate, false,
		pGameFrame->useColor ? (HGDIOBJ)hbsMass : (HGDIOBJ)hbmUnit,
		!pGameFrame->useColor);
}

void Drawer::DrawRollingLines(GameFrame* pGameFrame)
{
	if (GameState::RollUp == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
	{
		for (int i = pGameFrame->sizeY - pGameFrame->rolledRows;
			i < pGameFrame->sizeY; i++)
		{
			DrawLine(pGameFrame, i,
				pGameFrame->useColor ? (HGDIOBJ)hbsMass : (HGDIOBJ)hbmUnit,
				!pGameFrame->useColor);
		}
	}
}

void Drawer::DrawInfo(InfoFrame* pInfoFrame)
{
	if (!IsValid()) return;

	wstring labels;
	labels.append(L"Level\n\n");
	labels.append(L"Score\n\n");
	labels.append(L"StartLine\n\n");
	wstring infos;
	infos.append(to_wstring(pInfoFrame->GetLevel())).append(L"\n\n");
	infos.append(to_wstring(pInfoFrame->GetScore())).append(L"\n\n");
	infos.append(to_wstring(pInfoFrame->GetStartLine()));
	RECT rcInfo = { pInfoFrame->left, pInfoFrame->top,
		pInfoFrame->left + pInfoFrame->sizeX, pInfoFrame->top + pInfoFrame->sizeY };
	SetBkMode(hdcCmp, TRANSPARENT);
	SetTextColor(hdcCmp, pInfoFrame->colorInfo);
	SelectObject(hdcCmp, hftInfo);
	DrawText(hdcCmp, labels.c_str(), (int)labels.size(), &rcInfo, DT_LEFT);
	DrawText(hdcCmp, infos.c_str(), (int)infos.size(), &rcInfo, DT_RIGHT);
}

void Drawer::DrawIcon(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (GameState::Pause != pController->GetGameState() && GameState::ResumeDelay != pController->GetGameState())
		return;

	Image* pIcon = GameState::Pause == pController->GetGameState() ? pBitmapPause : pBitmapResume;

	REAL rate = pIcon->GetWidth() * pIcon->GetHeight() /
		(pGameFrame->GetWidth() * pGameFrame->GetHeight() * (REAL)pGameFrame->iconScaleRatio);
	rate = sqrt(rate);

	REAL width = pIcon->GetWidth() / rate;
	REAL height = pIcon->GetHeight() / rate;
	// ensure icon not out of game frame
	if (width > pGameFrame->GetWidth() || height > pGameFrame->GetHeight())
	{
		if (width / pGameFrame->GetWidth() >= height / pGameFrame->GetHeight())
		{
			rate = pIcon->GetWidth() * 1.0f / pGameFrame->GetWidth();
		}
		else
		{
			rate = pIcon->GetHeight() * 1.0f / pGameFrame->GetHeight();
		}
		width = pIcon->GetWidth() / rate;
		height = pIcon->GetHeight() / rate;
	}

	REAL left = (pGameFrame->GetWidth() - width) / 2 + pGameFrame->GetLeft();
	REAL top = (pGameFrame->GetHeight() - height) / 2 + pGameFrame->GetTop();

	Graphics graphics(hdcCmp);
	graphics.DrawImage(pIcon, left, top, width, height);
}

void Drawer::DrawMask(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (GameState::Pause != pController->GetGameState() && GameState::ResumeDelay != pController->GetGameState())
		return;
	
	Graphics graphics(hdcCmp);
	graphics.FillRectangle(pBrushMask, pGameFrame->GetLeft(), pGameFrame->GetTop(),
		pGameFrame->GetWidth(), pGameFrame->GetHeight());
}

void Drawer::DrawSplash(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (pController->IsStarted())
		return;
	
	HBITMAP hbmSplash = CreateHBITMAP(
		GameState::None == pController->GetGameState() ?
		pBitmapBegin : pBitmapGameOver);
	int left = pGameFrame->GetLeft() + pGameFrame->borderThickness;
	int top = pGameFrame->GetTop() + pGameFrame->borderThickness;
	int right = pGameFrame->GetLeft() + pGameFrame->GetWidth() - pGameFrame->borderThickness;
	int bottom = pGameFrame->GetTop() + pGameFrame->GetHeight() - pGameFrame->borderThickness;
	RECT rect = { left, top, right, bottom};
	DrawBitmap(hbmSplash, RenderMode::Uniform,
		RenderAlignment::HorizontalCenter | RenderAlignment::VerticalCenter,
		&rect);
	DeleteObject(hbmSplash);
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

void Drawer::GetDCResolution(HDC hdc, int* px, int* py)
{
	*px = GetDeviceCaps(hdc, LOGPIXELSX);
	*py = GetDeviceCaps(hdc, LOGPIXELSY);
}

void Drawer::SetBitmapDCResolution(Bitmap* pBitmap, HDC hdc)
{
	if (NULL == hdc)
		return;

	int resX, resY;
	GetDCResolution(hdc, &resX, &resY);
	pBitmap->SetResolution((REAL)resX, (REAL)resY);
}

COLORREF Drawer::LightColor(COLORREF color, double ratio)
{
	int r = (int)(GetRValue(color) * ratio + (0xFF) * (1 - ratio));
	int g = (int)(GetGValue(color) * ratio + (0xFF) * (1 - ratio));
	int b = (int)(GetBValue(color) * ratio + (0xFF) * (1 - ratio));
	return RGB(r, g, b);
}

HBITMAP Drawer::StretchBitmap(HDC hdcRef, HBITMAP hbm, int dstWidth, int dstHeight)
{
	Bitmap bitmap(hbm, NULL);
	int srcWidth = bitmap.GetWidth();
	int srcHeight = bitmap.GetHeight();

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdcRef);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdcRef, dstWidth, dstHeight);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdcRef);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdcRef, srcWidth, srcHeight);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// stretch bitmap
	Graphics graphics(hdcSrc);
	graphics.DrawImage(&bitmap, 0, 0, srcWidth, srcHeight);

	SetStretchBltMode(hdcDst, COLORONCOLOR);
	StretchBlt(hdcDst, 0, 0, dstWidth, dstHeight,
		hdcSrc, 0, 0, srcWidth, srcHeight,
		SRCCOPY);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::LightBitmap(HDC hdcRef, HBITMAP hbm, double ratio)
{
	Bitmap bitmap(hbm, NULL);
	UINT width = bitmap.GetWidth();
	UINT height = bitmap.GetHeight();

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdcRef);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdcRef, width, height);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdcRef);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdcRef, width, height);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// alpha blend bitmap
	FloodFill(hdcSrc, 0, 0, RGB(255, 255, 255));

	Graphics graphics(hdcDst);
	graphics.DrawImage(&bitmap, 0, 0, width, height);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = (BYTE)(0xFF * ratio);
	AlphaBlend(hdcDst, 0, 0, width, height,
		hdcSrc, 0, 0, width, height,
		bf);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::TranslateBitmap(HDC hdcRef, HBITMAP hbm, int offsetX, int offsetY)
{
	Bitmap bitmap(hbm, NULL);
	UINT width = bitmap.GetWidth();
	UINT height = bitmap.GetHeight();
	offsetX %= width;
	offsetY %= height;
	if (offsetX < 0)
		offsetX += width;
	if (offsetY < 0)
		offsetY += height;

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdcRef);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdcRef, width, height);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdcRef);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdcRef, width, height);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// translate bitmap
	Graphics graphics(hdcSrc);
	graphics.DrawImage(&bitmap, 0, 0, width, height);
	BitBlt(hdcDst, 0, 0, offsetX, offsetY,
		hdcSrc, width - offsetX, height - offsetY,
		SRCCOPY);
	BitBlt(hdcDst, offsetX, 0, width - offsetX, offsetY,
		hdcSrc, 0, height - offsetY,
		SRCCOPY);
	BitBlt(hdcDst, offsetX, offsetY, width - offsetX, height - offsetY,
		hdcSrc, 0, 0,
		SRCCOPY);
	BitBlt(hdcDst, 0, offsetY, offsetX, height - offsetY,
		hdcSrc, width - offsetX, 0,
		SRCCOPY);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::CreateHBITMAP(Bitmap* pBitmap)
{
	Color color;
	HBITMAP hbm;
	pBitmap->GetHBITMAP(color, &hbm);
	return hbm;
}

HBRUSH Drawer::GetRandomTetrisBrush()
{
	return vecTetrisBrushes[Utility::Random(0, (int)vecTetrisBrushes.size() - 1)];
}

bool Drawer::IsValid()
{
	return initialized && hdc && hdcCmp;
}

void Drawer::BackgroundFrameChangedProcStatic(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame)
{
	singleton.BackgroundFrameChangedProc(pBitmap, sLoopedCount, uCurrentFrame);
}

void Drawer::BackgroundFrameChangedProc(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame)
{
	if (RenderMode::Tile == pBackground->renderMode)
	{
		DeleteObject(hbmBackground);
		hbmBackground = CreateHBITMAP(pBitmap);
	}

	InvalidateRect(hWnd, NULL, FALSE);
}
