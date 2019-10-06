#include "TetrisType.h"

TetrisType::TetrisType(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, size_t color)
:group(NULL), name(NULL), pData(NULL)
{
	Initialize(group, name, penetrable, row, col, pData, dataCount, color);
}

bool TetrisType::Initialize(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, size_t color)
{
	size_t len;
	len = _tcslen(group);
	group = new TCHAR[len];
	this->group = group;
	this->name = name;
	this->penetrable = penetrable;
	this->row = row;
	this->col = col;
	this->pData = pData;
	this->color = color;
	return true;
}

void TetrisType::Append(TetrisType* pTetrisType)
{
}

void TetrisType::Remove(TetrisType* pTetirsType)
{
}
