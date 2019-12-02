#pragma once
#include <Unknwn.h>
#include <windef.h>
#include <gdiplus.h>
#include <map>
using namespace Gdiplus;
using namespace std;


class AnimatedGifPoller
{
public:
	typedef void (CALLBACK* FrameChangedProc)(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame);

	AnimatedGifPoller(Bitmap* pBitmap, FrameChangedProc pFrameChangedProc);
	~AnimatedGifPoller();

	bool Initialize();
	bool DeInitialize();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	Bitmap* GetBitmap();
	LONG GetFrameDelay(UINT uFrame); // in hundredths of a second
	SHORT GetLoopCount();
	void SetLoopInfinate(bool loopInfinate);

	static bool IsAnimatedGif(Bitmap* pBitmap);

private:
	bool IsGifFormat();
	bool ParseFrameCount(UINT* puFrameCount);
	bool ParseFrameDelays(LONG* const plFrameDelays);
	bool ParseLoopCount(SHORT* const psLoopCount);
	
	PropertyItem* CreatePropertyItem(PROPID propId);
	void DeletePropertyItem(PropertyItem* pPropertyItem);

	static void CALLBACK PollTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	void PollTimerProc(HWND hWnd, UINT msg, UINT_PTR id, DWORD millisecond);
	static map<UINT_PTR, AnimatedGifPoller*> mapTimers;

	bool m_bInitialized;                   // initialized
	Bitmap* m_pBitmap;                     // gif image
	UINT m_uFrameCount;                    // frame count
	LONG* m_plFrameDelays;                 // frame delay
	SHORT m_sLoopCount;                    // loop count, 0 forever
	SHORT m_sLoopedCount;                  // looped count
	bool m_bLoopInfinate;                  // loop infinate
	UINT m_uFrameCur;                      // current frame
	FrameChangedProc m_pFrameChangedProc;  // frame changed callback 
};

