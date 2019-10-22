#pragma once
#include <tchar.h>
#include "tcharstd.h"
#include <string>
#include "Configuration.h"
using namespace std;

class Background
{
public:
	void Initialize(Configuration* pConfiguration);
	static Background singleton;

	tstring pathBackground;
	COLORREF color;
	bool useBackground;

private:
	Background() {};
};

