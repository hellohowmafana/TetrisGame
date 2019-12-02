#pragma once
class Frame
{
public:
	int left;
	int top;
	int sizeX;
	int sizeY;

	virtual ~Frame() = default;

	virtual int GetLeft();
	virtual int GetTop();
	virtual int GetWidth();
	virtual int GetHeight();
};

