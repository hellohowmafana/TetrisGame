#include "TetrisType.h"

void TetrisType::Create(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
{
	lstTetrisTypes.push_back(new TetrisType(group, name, penetrable, row, col, pData, dataCount, color));
}

void TetrisType::Delete(TetrisType* pTetrisType)
{
	lstTetrisTypes.remove(pTetrisType);
	delete pTetrisType;
}

void TetrisType::Clear(TetrisType* pTetirsType)
{
	for (list<TetrisType*>::iterator it = lstTetrisTypes.begin(); it != lstTetrisTypes.end(); it++)
	{
		delete* it;
	}
	lstTetrisTypes.clear();
}

TetrisType::TetrisType(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
:group(NULL), name(NULL), pData(NULL)
{
	Initialize(group, name, penetrable, row, col, pData, dataCount, color);
}

TetrisType::~TetrisType()
{
	delete[] group;
	delete[] name;
	delete[] pData;
}

bool TetrisType::Initialize(TCHAR* group, TCHAR* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
{
	size_t len;
	
	len = _tcslen(group);
	this->group = new TCHAR[len];
	_tcscpy(this->group, group);
	
	len = _tcslen(name);
	this->name = new TCHAR[len];
	_tcscpy(this->name, name);

	this->penetrable = penetrable;
	this->row = row;
	this->col = col;
	
	this->pData = new char[dataCount];
	memcpy(this->pData, pData, dataCount);
	
	this->color = color;

	int centerPos = (int)(find(pData, pData + dataCount, '2') - pData);
	if (centerPos == dataCount)
	{
		canRotate = false;
	}
	else
	{
		canRotate = true;
		GetXY(centerPos, &(this->centerX), &(this->centerY));
	}

	return true;
}

int TetrisType::GetPostion(int x, int y)
{
	if (x >= col || y >= row)
		return -1;
	return x * col + y;
}

void TetrisType::GetXY(int pos, int* px, int* py)
{
	*px = pos % col;
	*py = pos / col;
}

list<TetrisType*> TetrisType::lstTetrisTypes;