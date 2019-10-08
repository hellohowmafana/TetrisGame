#include "Accretion.h"
#include "Configuration.h"
#include "Utility.h"

Accretion::Accretion()
:top(Configuration::singleton.frameSizeY - 1)
{
}

void Accretion::Insert(TetrisShape* pTetrisShape)
{
	int topTetrisShape = pTetrisShape->GetTop();
	if (top > topTetrisShape)
	{
		for (int i = 0; i < top - topTetrisShape; i++)
		{
			AccretionLine* pAccretionLine = CreateLine();
			for (int j = 0; j < pTetrisShape->GetRight(); j++)
			{
				(*pAccretionLine)[j].isSolid = pTetrisShape->IsSolid(j, pTetrisShape->GetHeight() - 1 - i);
				(*pAccretionLine)[j].color = (char)(pTetrisShape->GetColor());
			}
			InsertLine(0, pAccretionLine);
		}
	}
}

void Accretion::RemoveLine(int line)
{
	DeleteLine(top - line);
}

void Accretion::GenerateLine(int line, int blankRate)
{
	if (line <= 0 || line >= top)
		return;

	AccretionLine* pAccretionLine = CreateLine();
	for (size_t i = 0; i < pAccretionLine->size(); i++)
	{
		(*pAccretionLine)[i].color = -1;
		(*pAccretionLine)[i].isSolid = Utility::RandomTrue(1 - Configuration::singleton.startLineBlankRate);
	}
	InsertLine(0, pAccretionLine);

	// substract line from top more than 1
	int diff = top - line - 1;
	while (diff--)
	{
		InsertLine(1, CreateLine());
	}
}

int Accretion::GetTop()
{
	return top;
}

bool Accretion::IsSolid(int x, int y)
{
	return GetUnit(x, y)->isSolid;
}

AccretionLine* Accretion::CreateLine()
{
	AccretionUnit accretionUnit;
	AccretionLine* pAccretionLine = new AccretionLine(Configuration::singleton.frameSizeX, accretionUnit);
	return pAccretionLine;
}

void Accretion::InsertLine(int at, AccretionLine* pAccretionLine)
{
	AccretionBlock::iterator it = accretionBlock.begin();
	while (at--) {
		it++;
	}
	accretionBlock.insert(it, pAccretionLine);
}

void Accretion::InsertLines(int at, AccretionBlock* pAccretionBlock)
{
	AccretionBlock::iterator it = accretionBlock.begin();
	while (at--) {
		it++;
	}
	accretionBlock.insert(it, pAccretionBlock->begin(), pAccretionBlock->end());
}

void Accretion::DeleteLine(int at)
{
	AccretionBlock::iterator it = accretionBlock.begin();
	while (at--)
	{
		it++;
	}
	delete* it;
	accretionBlock.erase(it);
}

void Accretion::DeleteLines(int at, int count)
{
	AccretionBlock::iterator it = accretionBlock.begin();
	while (at--)
	{
		it++;
	}
	while (count--)
	{
		delete* it;
		accretionBlock.erase(it);
		it++;
	}
}

AccretionUnit* Accretion::GetUnit(int x, int y)
{
	if (x < 0 || x >= Configuration::singleton.frameSizeX
		|| y <= 0 || y >= Configuration::singleton.frameSizeY)
		return nullptr;

	return &(GetLine(y)->at(x));
}

AccretionLine* Accretion::GetLine(int line)
{
	if (line <= 0 || line >= Configuration::singleton.frameSizeY)
		return nullptr;

	int offset = line - top;
	AccretionBlock::iterator it = accretionBlock.begin();
	while (offset--)
	{
		it++;
	}

	return *it;
}
