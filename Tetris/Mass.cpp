#include "Mass.h"
#include "TetrisShape.h"
#include "GameFrame.h"
#include "Utility.h"

bool Mass::Initialize()
{
	if (!pGameFrame) return false;
	top = pGameFrame->sizeY;
	return true;
}

bool Mass::IsEmpty()
{
	return pGameFrame->sizeY == top;
}

int Mass::GetTopmostSolidY(int x, int yFrom)
{
	for (int i = yFrom; i < pGameFrame->sizeY; i++)
	{
		if (IsSolid(x, i))
			return i;
	}
	return -1;
}

bool Mass::IsTouched(TetrisShape* pTetrisShape)
{
	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		if (pTetrisShape->GetBottommostSolidY(i, true) == pGameFrame->sizeY ||
			pTetrisShape->GetBottommostSolidY(i, true) == GetTopmostSolidY(i, pTetrisShape->GetBottom()))
			return true;
	}

	return false;
}

int Mass::CalculateDistanceY(TetrisShape* pTetrisShape)
{
	int minimal = pGameFrame->sizeY;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		int massTopmostSolidY = GetTopmostSolidY(i, pTetrisShape->GetBottom() + 1);
		if(-1 == massTopmostSolidY)
			massTopmostSolidY = pGameFrame->sizeY;

		int shapeBottommostSolidY = pTetrisShape->GetBottommostSolidY(i, true);
		if (-1 == shapeBottommostSolidY)
			continue;

		int distance = massTopmostSolidY - shapeBottommostSolidY;
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

bool Mass::HitTest(TetrisShape* pTetrisShape)
{
	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if (pTetrisShape->IsSolid(i, j, true) && IsSolid(i, j))
				return true;
		}
	}
	return false;
}

bool Mass::Union(TetrisShape* pTetrisShape)
{
	if (pTetrisShape->GetGameFrame() != GetGameFrame())
		return false;

	int topTetrisShape = pTetrisShape->GetTop();
	if (top > topTetrisShape)
	{
		int count = top - topTetrisShape;
		while (count--)
		{
			InsertLine(0, CreateLine());
		}
		top = topTetrisShape;
	}

	MassBlock::iterator it = GetLineIterator(pTetrisShape->GetTop());
	for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
	{
		for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
		{
			bool isSolid = pTetrisShape->IsSolid(i, j, true);
			if (isSolid)
			{
				(*it)->at(i).isSolid = isSolid;
				(*it)->at(i).color = (char)pTetrisShape->GetColor();
			}
		}
		it++;
	}
	return true;
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
	if (!ValidateXY(x, y))
		return false;
	return GetUnit(x, y)->isSolid;
}

bool Mass::IsSolid(MassLine* pMassLine, int x)
{
	if (!ValidateX(x))
		return false;
	return pMassLine->at(x).isSolid;
}

void Mass::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

GameFrame* Mass::GetGameFrame()
{
	return pGameFrame;
}

MassUnit* Mass::GetUnit(int x, int y)
{
	if (!ValidateXY(x, y))
		return nullptr;

	return &(GetLine(y)->at(x));
}

MassLine* Mass::GetLine(int line)
{
	MassBlock::iterator it = GetLineIterator(line);
	if (massBlock.end() == it)
		return nullptr;

	return *it;
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

MassBlock::iterator Mass::GetTopLineIterator()
{
	return massBlock.begin();
}

MassBlock::iterator Mass::GetBottomLineIterator()
{
	return massBlock.end();
}

MassLine* Mass::CreateLine()
{
	MassUnit massUnit;
	MassLine* pMassLine = new MassLine(pGameFrame->sizeX, massUnit);
	return pMassLine;
}

void Mass::InsertLine(int at, MassLine* pMassLine)
{
	massBlock.insert(next(massBlock.begin(), at), pMassLine);
}

void Mass::InsertLines(int at, MassBlock* pMassBlock)
{
	massBlock.insert(next(massBlock.begin(), at), pMassBlock->begin(), pMassBlock->end());
}

void Mass::DeleteLine(int at)
{
	MassBlock::iterator it = next(massBlock.begin(), at);
	delete *it;
	massBlock.erase(it);
}

void Mass::DeleteLines(int at, int count)
{
	MassBlock::iterator it = next(massBlock.begin(), at);
	while (count--)
	{
		delete *it;
		massBlock.erase(it);
		it++;
	}
}

void Mass::ClearLines()
{
	for (MassBlock::iterator it = massBlock.begin(); it != massBlock.end(); it++)
	{
		delete* it;
	}
	massBlock.clear();
}

bool Mass::Save(TCHAR* szString)
{
	return false;
}

bool Mass::Load(TCHAR* szString)
{
	tstring str;
	tistringstream stringstream(szString);
	getline(stringstream, str);
	if (tstring::npos != str.find(Archive::labelMass))
	{
		while (!stringstream.eof())
		{
			getline(stringstream, str);
			MassLine* pMassLine = CreateLine();
			TCHAR** szUnits = new TCHAR * [pGameFrame->sizeX];
			Utility::SplitString((TCHAR*)(str.c_str()), _T(' '), szUnits, pGameFrame->sizeX);
			for (size_t i = 0; i < str.size(); i++)
			{
				pMassLine->at(i).isSolid = szUnits[i][0] == _T('1');
				pMassLine->at(i).color = stoi(szUnits[i] + 2);
			}
			delete[] szUnits;
			InsertLine(massBlock.size(), pMassLine);
		}
	}
	else
	{
		return false;
	}
	return true;
}
