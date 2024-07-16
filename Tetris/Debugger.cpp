#if _DEBUG
#include "Debugger.hpp"
#include "Drawer.hpp"

void Debugger::SetDebugText(const wchar_t* pcText)
{
	Drawer::SetDebugText(pcText);
}
#endif
