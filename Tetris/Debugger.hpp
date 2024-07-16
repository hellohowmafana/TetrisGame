#pragma once
#if _DEBUG
class Debugger
{
public:
	static void SetDebugText(const wchar_t* pcText);
};
#endif
