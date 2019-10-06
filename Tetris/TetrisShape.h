#pragma once
#include "TetrisType.h"
#include <vector>
#include <tchar.h>
using namespace std;

class TetrisShape
{
private:
	void From(TetrisType* pType);

public:
	void Rotate();
	void Move();
	void StepDown();
	void DropDown();

private:
	
	int width, height;
	int centerX, centerY; // for rotate center
};

