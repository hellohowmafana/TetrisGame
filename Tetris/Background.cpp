#include "Background.hpp"
#include "Configuration.hpp"

void Background::Initialize(Configuration* pConfiguration)
{
	colorBackground = pConfiguration->colorBackground;
	pathBackground = pConfiguration->pathBackground;
	renderMode = pConfiguration->backgroundMode;
	renderAlignmentHorizontal = pConfiguration->backgroundAlignmentHorizontal;
	renderAlignmentVertical = pConfiguration->backgroundAlignmentVertical;
}

Background Background::singleton;
