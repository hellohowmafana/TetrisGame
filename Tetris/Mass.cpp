#include "Mass.hpp"
#include "TetrisShape.hpp"
#include "GameFrame.hpp"
#include "Utility.hpp"
#include <sstream>
#include "BinarySerializer.h"

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
			IsSolid(i, pTetrisShape->GetBottommostSolidY(i, true)))
		{
			return true;
		}
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

int Mass::FindBottommostBlankY(TetrisShape* pTetrisShape, int x)
{
	TetrisShape testTetrisShape;
	pTetrisShape->Clone(&testTetrisShape);
	for (int i = GetBottom() - pTetrisShape->GetHeight() + 1; i >= 0; i--)
	{
		testTetrisShape.MoveTo(x, i);
		if (!HitTest(&testTetrisShape))
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
	if (pTetrisShape->GetFrame() != (UnitFrame*)GetGameFrame())
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

bool Mass::IsLineFull(int line)
{
	return IsLineFull(GetLine(line));
}

bool Mass::IsLineFull(MassLine* pMassLine)
{
	for (MassLine::iterator it = pMassLine->begin(); it != pMassLine->end(); it++)
	{
		if (!it->isSolid)
			return false;
	}
	return true;
}

bool Mass::HasFullLine()
{
	for (MassBlock::iterator it = massBlock.begin(); it != massBlock.end(); it++)
	{
		if (IsLineFull(*it))
			return true;
	}
	return false;
}

void Mass::RemoveLine(int line)
{
	if (!TestY(line)) return;
	DeleteLine(line - top);
	top++;
}

int Mass::RemoveFullLines(int from, int to)
{
	if (!TestY(from) || !TestY(to)) return 0;
	if (from > to)
		swap(from, to);
	MassBlock::iterator it = GetLineIterator(from);
	MassBlock::iterator itto = next(it, to - from + 1);
	int count = 0;
	while (it != itto)
	{
		if (IsLineFull(*it))
		{
			DeleteLine(*it++); // increment it before delete
			count++;
		}
		else
		{
			it++;
		}
	}
	top += count;
	return count;
}

bool Mass::GenerateLine(int line, float blankRate)
{
	if (line <= 0 || line >= top)
		return false;

	MassLine* pMassLine = CreateLine();
	for (size_t i = 0; i < pMassLine->size(); i++)
	{
		(*pMassLine)[i].color = -1;
		(*pMassLine)[i].isSolid = Utility::RandomTrue(1.0f - blankRate);
	}
	InsertLine(0, pMassLine);

	// create lines from generated line to top
	int diff = top - line - 1;
	while (diff--)
	{
		InsertLine(1, CreateLine());
	}

	top = line;

	return true;
}

bool Mass::GenerateLines(int line, int count, float blankRate)
{
	if (line <= 0 || line + count - 1 >= top)
		return false;

	for (int i = 0; i < count; i++)
	{
		MassLine* pMassLine = CreateLine();
		for (size_t i = 0; i < pMassLine->size(); i++)
		{
			(*pMassLine)[i].color = -1;
			(*pMassLine)[i].isSolid = Utility::RandomTrue(1.0f - blankRate);
		}
		InsertLine(0, pMassLine);
	}

	// create lines from generated lines to top
	int diff = top - line - count;
	while (diff--)
	{
		InsertLine(1, CreateLine());
	}

	top = line;

	return true;
}

void Mass::Clear()
{
	ClearLines();
	top = pGameFrame->sizeY;
}

bool Mass::IsFull()
{
	return 0 == top;
}

vector<MassLine*>* Mass::GetFullLines(vector<MassLine*>* pvecLines, int from, int to)
{
	pvecLines->clear();
	for (int i = from; i <= to; i++)
	{
		if(IsLineFull(i))
			pvecLines->push_back(GetLine(i));
	}
	return pvecLines;
}

bool Mass::TestX(int x)
{
	return x >= 0 && x < pGameFrame->sizeX;
}

bool Mass::TestY(int y)
{
	return y >= top && y < pGameFrame->sizeY;
}

bool Mass::TestXY(int x, int y)
{
	return TestX(x) && TestY(y);
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
	if (!TestXY(x, y))
		return false;
	return GetUnit(x, y)->isSolid;
}

bool Mass::IsSolid(MassLine* pMassLine, int x)
{
	if (!TestX(x))
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
	if (!TestXY(x, y))
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
	if (!TestY(line))
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
	delete* it;
	massBlock.erase(it);
}

void Mass::DeleteLine(MassLine* pMassLine)
{
	delete pMassLine;
	massBlock.erase(find(massBlock.begin(), massBlock.end(), pMassLine));
}

void Mass::DeleteLines(int at, int count)
{
	MassBlock::iterator it = next(massBlock.begin(), at);
	while (count--)
	{
		delete* it;
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

bool Mass::Save(const wstring label, wstring& value)
{
	if (Archive::labelMass == label)
	{
		wstring strMass;
		strMass = L"\n";
		for (MassBlock::iterator itb = massBlock.begin(); itb != massBlock.end(); itb++)
		{
			MassLine* pMassLine = *itb;
			for (MassLine::iterator itl = pMassLine->begin(); itl != pMassLine->end() ; itl++)
			{
				strMass.append(itl->isSolid ? L"1" : L"0");
				strMass.append(L",");
				strMass.append(to_wstring(itl->color));
				strMass.append(L" ");
			}
			strMass.erase(strMass.end() - 1);
			strMass.append(L"\n");
		}
		strMass.erase(strMass.end() - 1);
		value = strMass;
	}
	else
	{
		return false;
	}

	return true;
}

bool Mass::Load(const wstring label, wstring value)
{
	wstring str;
	wistringstream stringstream(value);
	if (Archive::labelMass == label)
	{
		ClearLines();
		while (!stringstream.eof())
		{
			getline(stringstream, str);
			MassLine* pMassLine = CreateLine();
			wchar_t** szUnits = new wchar_t * [pGameFrame->sizeX];
			Utility::SplitString((wchar_t*)(str.c_str()), L' ', szUnits, pGameFrame->sizeX);
			for (int i = 0; i < pGameFrame->sizeX; i++)
			{
				pMassLine->at(i).isSolid = szUnits[i][0] == L'1';
				pMassLine->at(i).color = stoi(szUnits[i] + 2);
			}
			delete[] szUnits;
			InsertLine((int)massBlock.size(), pMassLine);
		}
		top = pGameFrame->sizeY - (int)massBlock.size();
	}
	else
	{
		return false;
	}
	return true;
}

bool Mass::Save(char*& pData, unsigned int& size, char argument)
{
	typedef map<char, vector<unsigned short>> ColorUnits;
	ColorUnits colorUnits;
	for (MassBlock::iterator itb = massBlock.begin(); itb != massBlock.end(); itb++)
	{
		MassLine* pMassLine = *itb;
		for (MassLine::iterator itl = pMassLine->begin(); itl != pMassLine->end(); itl++)
		{
			if (itl->isSolid)
				colorUnits[itl->color].push_back((unsigned short)
					(((top + distance(itb, massBlock.begin())) << 8) +
					(distance(itl, pMassLine->begin()))));
		}
	}
	BinarySerializer binarySerializer;
	binarySerializer.PutUshort(pData, static_cast<int>(colorUnits.size()), size);
	for (ColorUnits::iterator itc = colorUnits.begin(); itc != colorUnits.end(); itc++)
	{
		vector<unsigned short>& units = itc->second;
		binarySerializer.PutUshort(pData, static_cast<int>(units.size()), size);
		binarySerializer.PutChar(pData, itc->first, size);
		for (vector<unsigned short>::iterator itu = units.begin(); itu != units.end(); itu++)
		{
			binarySerializer.PutUshort(pData, *itu, size);
		}
	}

	return true;
}

bool Mass::Load(char*& pData, char argument)
{
	typedef map<char, vector<unsigned short>> ColorUnits;
	ColorUnits colorUnits;
	BinarySerializer binarySerializer;
	int colorUnitsSize;
	binarySerializer.TakeUshort(pData, colorUnitsSize);
	for (int i = 0; i < colorUnitsSize; i++)
	{
		int unitsSize;
		binarySerializer.TakeUshort(pData, unitsSize);
		int color;
		binarySerializer.TakeChar(pData, color);
		for (int j = 0; j < unitsSize; j++)
		{
			int unit;
			binarySerializer.TakeUshort(pData, unit);
			colorUnits[(char)color].push_back((unsigned short)unit);
		}
	}

	return true;
}
