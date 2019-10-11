#include "Mass.h"
#include "TetrisShape.h"
#include "GameFrame.h"
#include "Utility.h"

bool Mass::HasTouched(TetrisShape* pTetrisShape)
{
	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		if (pTetrisShape->GetBottommostSolidY(i) + 1 == GetTopmostSolidY(i))
			return true;
	}

	return false;
}

int Mass::DistanceFrom(TetrisShape* pTetrisShape)
{
	int minimal = 0;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		int distance = GetTopmostSolidY(i) - pTetrisShape->GetBottommostSolidY(i);
		if (distance < minimal)
			minimal = distance;
	}

	return minimal;
}

bool Mass::CanInsert(TetrisShape* pTetrisShape, int x, int y)
{
	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j < pTetrisShape->GetBottom(); j++)
		{
			if (!(!IsSolid(x, y) && pTetrisShape->IsSolid(i, j, true)))
				return false;
		}
	}
	return true;
}

int Mass::FindBottommostBlankY(TetrisShape* pTetrisShape, int x)
{
	for (int i = GetBottom() - pTetrisShape->GetHeight() + 1; i <= top; i--)
	{
		if (CanInsert(pTetrisShape, x, i))
			return i;
	}
	return -1;
}

bool Mass::HasBlank(TetrisShape* pTetrisShape, int x)
{
	return FindBottommostBlankY(pTetrisShape, x) != -1;
}

void Mass::Union(TetrisShape* pTetrisShape)
{
	int topTetrisShape = pTetrisShape->GetTop();
	if (top > topTetrisShape)
	{
		int count = top - topTetrisShape;
		while (count--)
		{
			InsertLine(0, CreateLine());
		}
	}
	MassBlock::iterator it = GetLineIterator(pTetrisShape->GetTop());
	for (int j = GetTop(); j <= GetBottom(); j++)
	{
		for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
		{
			MassLine* pMassLine = *it;
			(*pMassLine)[i].isSolid = pTetrisShape->IsSolid(i, j, true);
			(*pMassLine)[i].color = (char)pTetrisShape->GetColor();
		}
		it++;
	}
}

void Mass::RemoveLine(int line)
{
	if (!ValidateY(line)) return;
	DeleteLine(line - top);
}

void Mass::GenerateLine(int line, int blankRate)
{
	if (line <= 0 || line >= top)
		return;

	MassLine* pMassLine = CreateLine();
	for (size_t i = 0; i < pMassLine->size(); i++)
	{
		(*pMassLine)[i].color = -1;
		(*pMassLine)[i].isSolid = Utility::RandomTrue(1.0 - blankRate);
	}
	InsertLine(0, pMassLine);

	// substract line from top more than 1
	int diff = top - line - 1;
	while (diff--)
	{
		InsertLine(1, CreateLine());
	}
}

bool Mass::ValidateX(int x)
{
	return x >= 0 && x < pGameFrame->sizeX;
}

bool Mass::ValidateY(int y)
{
	return y >= top && y < pGameFrame->sizeY;
}

bool Mass::ValidateXY(int x, int y)
{
	return ValidateX(x) && ValidateY(y);
}

int Mass::GetTop()
{
	return top;
}

int Mass::GetBottom()
{
	return pGameFrame->sizeY - 1;
}

int Mass::GetRight()
{
	return pGameFrame->sizeX - 1;
}

int Mass::GetHeight()
{
	return pGameFrame->sizeY - top;
}

bool Mass::IsSolid(int x, int y)
{
	return GetUnit(x, y)->isSolid;
}

bool Mass::IsSolid(MassLine* pMassLine, int x)
{
	return pMassLine->at(x).isSolid;
}

int Mass::GetTopmostSolidY(int x)
{
	for (int i = top; i < pGameFrame->sizeY; i++)
	{
		if (IsSolid(x, i))
			return i;
	}
	return -1;
}

void Mass::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

MassUnit* Mass::GetUnit(int x, int y)
{
	if (!ValidateXY(x, y))
		return nullptr;

	return &(GetLine(y)->at(x));
}

MassBlock::iterator Mass::GetLineIterator(int line)
{
	if (!ValidateY(line))
	{
		return massBlock.end();
	}

	int offset = line - top;
	return next(massBlock.begin(), offset);
}

MassBlock::iterator Mass::GetFirstLineIterator()
{
	return massBlock.begin();
}

MassBlock::iterator Mass::GetLastLineIterator()
{
	return massBlock.end();
}

MassLine* Mass::GetLine(int line)
{
	MassBlock::iterator it = GetLineIterator(line);
	if (massBlock.end() == it)
		return nullptr;

	return *it;
}

MassLine* Mass::CreateLine()
{
	MassUnit massUnit;
	MassLine* pMassLine = new MassLine(pGameFrame->sizeX, massUnit);
	return pMassLine;
}

void Mass::InsertLine(int at, MassLine* pMassLine)
{
	MassBlock::iterator it = massBlock.begin();
	while (at--) {
		it++;
	}
	massBlock.insert(it, pMassLine);
}

void Mass::InsertLines(int at, MassBlock* pMassBlock)
{
	MassBlock::iterator it = massBlock.begin();
	while (at--) {
		it++;
	}
	massBlock.insert(it, pMassBlock->begin(), pMassBlock->end());
}

void Mass::DeleteLine(int at)
{
	MassBlock::iterator it = massBlock.begin();
	while (at--)
	{
		it++;
	}
	delete* it;
	massBlock.erase(it);
}

void Mass::DeleteLines(int at, int count)
{
	MassBlock::iterator it = massBlock.begin();
	while (at--)
	{
		it++;
	}
	while (count--)
	{
		delete* it;
		massBlock.erase(it);
		it++;
	}
}