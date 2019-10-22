#pragma once
#include <Unknwn.h>
#include <windef.h>
#include <gdiplus.h>
#include <map>
using namespace Gdiplus;
using namespace std;

class AnimatedGifPlayer
{
public:
	AnimatedGifPlayer();
	AnimatedGifPlayer(const TCHAR* szFilePath);
	~AnimatedGifPlayer();

	bool Initialize(const TCHAR* szFilePath);
	bool DeInitialize();
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void SetRedrawRegion(HWND hWnd, HRGN hRgn);
	Image* GetImage();
	LONG GetFrameDelay(UINT uFrame); // in hundredths of a second
	SHORT GetLoopCount();
	void SetLoopInfinate(bool loopInfinate);

private:
	PropertyItem* CreatePropertyItem(PROPID propId);
	void DeletePropertyItem(PropertyItem* pPropertyItem);

	static void CALLBACK TimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void TimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	static map<UINT_PTR, AnimatedGifPlayer*> mapTimers;

	Bitmap* m_pBitmap;               // gif image
	TCHAR m_szFilePath[MAX_PATH];    // gif file path
	UINT m_uFrameCount;              // frame count
	PropertyItem* m_pPropFrameDelay; // frame delay property
	SHORT m_sLoopCount;              // loop count
	SHORT m_sLoopedCount;            // looped count
	bool m_bLoopInfinate;            // loop infinate
	UINT m_uFrameCur;                // current frame
	HWND m_hWndRedraw;               // redraw window
	HRGN m_hRgnRedraw;               // redraw region
};

