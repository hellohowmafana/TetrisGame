#include "Background.h"
#include "Configuration.h"

void Background::Initialize(Configuration* pConfiguration)
{
	colorBackground = pConfiguration->colorBackground;
	pathBackground = pConfiguration->pathBackground;
	backgroundMode = pConfiguration->backgroundMode;
}

Background Background::singleton;
