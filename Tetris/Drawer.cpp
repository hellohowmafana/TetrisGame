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
	try {
		this->pController = pController;
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
		if (Status::Ok != pBitmapBackground->GetLastStatus())
			return false;
		SetBitmapDCResolution(pBitmapBackground, NULL);
		pBitmapBackground->GetHBITMAP(Color(), &hbmBackground);
		hbsBackground = BackgroundMode::Tile == pBackground->backgroundMode ?
			CreatePatternBrush(hbmBackground) :
			CreateSolidBrush(pBackground->colorBackground);
		if (AnimatedGifPoller::IsAnimatedGif(pBitmapBackground))
		{
			pAnimatedGifPoller = new AnimatedGifPoller(pBitmapBackground, BackgroundFrameChangedProcStatic);
			pAnimatedGifPoller->SetLoopInfinate(true);
			pAnimatedGifPoller->Start();
		}
		isAnimatedBackground = AnimatedGifPoller::IsAnimatedGif(pBitmapBackground);

		hftInfo = CreateFont(pInfoFrame->fontHeight, pInfoFrame->fontWidth, 0, 0,
			pInfoFrame->fontWeight*100, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_DONTCARE, pInfoFrame->fontFace.c_str());

		pBitmapGameOver = new Bitmap(pGameFrame->pathGameOver.c_str());
		if (Status::Ok != pBitmapGameOver->GetLastStatus())
			return false;

		pBitmapPause = new Bitmap(pGameFrame->pathPauseIcon.c_str());
		if (Status::Ok != pBitmapPause->GetLastStatus())
			return false;
		pBitmapResume = new Bitmap(pGameFrame->pathResumeIcon.c_str());
		if (Status::Ok != pBitmapResume->GetLastStatus())
			return false;

		pBrushMask = new SolidBrush(Color((BYTE)(255 * pGameFrame->maskTransparency), 255, 255, 255));
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
	for (size_t i = 0; i < vecTetrisBrushes.size(); i++)
	{
		DeleteObject(vecTetrisBrushes.at(i));
	}
	vecTetrisBrushes.clear();
	DeleteObject(hbsMass);
	hbsMass = NULL;

	delete pBitmapBackground;
	pBitmapBackground = nullptr;
	DeleteObject(hbmBackground);
	hbmBackground = NULL;
	DeleteObject(hbsBackground);
	hbsBackground = NULL;
	if (pAnimatedGifPoller)
	{
		delete pAnimatedGifPoller;
		pAnimatedGifPoller = nullptr;
	}

	DeleteObject(hftInfo);
	hftInfo = NULL;

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
		DrawFrame(pPromptFrame);
		DrawFrame(pInfoFrame);
		DrawInfo(pInfoFrame);

		if (pController->IsStarted())
		{
			DrawShape(pGameFrame, pGameFrame->GetShape());
			DrawMass(pGameFrame, pGameFrame->GetMass());
			DrawRollingLines(pGameFrame);
			DrawMask(pGameFrame);
			DrawIcon(pGameFrame);
		
			DrawShape(pPromptFrame, pPromptFrame->GetTerisShape());
		}
		else
		{
			DrawFill(pGameFrame, 0);
		}

		BitBlt(hdc, 0, 0, dcWidth, dcHeight, hdcCmp, 0, 0, SRCCOPY);
	}
}

void Drawer::Invalidate()
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
				DrawUnit(pUnitFrame, i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
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
		HBRUSH hbs;
		if (GameState::BlinkLight == pController->GetGameState() && pGameFrame->IsLastFullLine(pMassLine))
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

void Drawer::DrawLine(UnitFrame* pUnitFrame, int y, HBRUSH brush)
{
	for (int i = 0; i < pUnitFrame->sizeX; i++)
	{
		DrawUnit(pUnitFrame, i, y, brush);
	}
}

void Drawer::DrawUnits(UnitFrame* pUnitFrame, double blankRate, bool leanBlank, HBRUSH brush)
{
	if (1 == blankRate)
		return;

	if (0 == blankRate)
	{
		for (int i = 0; i < pUnitFrame->GetWidth(); i++)
		{
			for (int j = 0; j < pUnitFrame->GetHeight(); j++)
			{
				DrawUnit(pUnitFrame, i, j, brush);
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
				DrawUnit(pUnitFrame, x, y, brush);
			}
		}
	}
}

void Drawer::DrawFill(UnitFrame* pUnitFrame, double blankRate)
{
	DrawUnits(pUnitFrame, blankRate, false, hbsMass);
}

void Drawer::DrawRollingLines(GameFrame* pGameFrame)
{
	if (GameState::RollUp == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
	{
		for (int i = pGameFrame->sizeY - pGameFrame->rolledRows;
			i < pGameFrame->sizeY; i++)
		{
			DrawLine(pGameFrame, i, hbsMass);
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
	infos.append(to_wstring(*pInfoFrame->pLevel)).append(L"\n\n");
	infos.append(to_wstring(*pInfoFrame->pScore)).append(L"\n\n");
	infos.append(to_wstring(*pInfoFrame->pStartLine));
	RECT rcInfo = { pInfoFrame->left, pInfoFrame->top,
		pInfoFrame->left + pInfoFrame->sizeX, pInfoFrame->top + pInfoFrame->sizeY };
	SetBkMode(hdcCmp, TRANSPARENT);
	SetTextColor(hdcCmp, pInfoFrame->colorInfo);
	SelectObject(hdcCmp, hftInfo);
	DrawText(hdcCmp, labels.c_str(), labels.size(), &rcInfo, DT_LEFT);
	DrawText(hdcCmp, infos.c_str(), infos.size(), &rcInfo, DT_RIGHT);
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
	int resX, resY;
	bool useScreenDC;
	if (NULL == hdc)
	{
		useScreenDC = true;
		hdc = GetDC(NULL);
	}
	GetDCResolution(hdc, &resX, &resY);
	if (useScreenDC)
		DeleteDC(hdc);
	pBitmap->SetResolution((REAL)resX, (REAL)resY);
}

HBRUSH Drawer::GetRandomTetrisBrush()
{
	return vecTetrisBrushes[Utility::Random(0, vecTetrisBrushes.size() - 1)];
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
	if (BackgroundMode::Tile == pBackground->backgroundMode)
	{
		DeleteObject(hbmBackground);
		pBitmap->GetHBITMAP(Color(), &hbmBackground);
		DeleteObject(hbsBackground);
		hbsBackground = CreatePatternBrush(hbmBackground);
	}

	InvalidateRect(hWnd, NULL, FALSE);
}
