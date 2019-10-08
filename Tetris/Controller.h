#pragma once
#include "TetrisShape.h"

class Controller
{
public:
	void Rotate(TetrisShape* pTetrisShape);
	void StepDown(TetrisShape* pTetrisShape);
	void Drop(TetrisShape* pTetrisShape);

};

