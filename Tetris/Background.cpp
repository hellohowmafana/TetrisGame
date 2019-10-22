#include "Background.h"

void Background::Initialize(Configuration* pConfiguration)
{
	color = pConfiguration->colorBackground;
	pathBackground = pConfiguration->pathBackground;
	useBackground = pConfiguration->useBackground;
}

Background Background::singleton;
