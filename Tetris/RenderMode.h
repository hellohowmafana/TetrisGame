#pragma once
#include <Windows.h>

enum class RenderMode
{
	Color = 0, General = 1, Strech = 2, Tile = 3, UniformFill = 4, Uniform = 5
};

enum class RenderAlignment
{
	HorizontalLeft = 0x00000001,
	HorizontalRight = 0x00000010,
	HorizontalCenter = 0x00000100,
	VerticalLeft = 0x00001000,
	VerticalRight = 0x00010000,
	VerticalCenter = 0x00100000,
};

DEFINE_ENUM_FLAG_OPERATORS(RenderAlignment)

