#include "Helper.hpp"

void Helper::AdjustWindow(HWND hWnd, Configuration * pConfiguration)
{
	Utility::ResizeWindow(hWnd, pConfiguration->windowWidth, pConfiguration->windowHeight);
	Utility::ResizableWindow(hWnd, false);
	if (pConfiguration->windowCenter)
		Utility::CenterWindow(hWnd);
	else
		Utility::MoveWindow(hWnd, pConfiguration->windowLeft, pConfiguration->windowTop);	
}
