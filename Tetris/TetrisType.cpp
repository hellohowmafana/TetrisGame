#include "TetrisType.h"
#include "Utility.h"

void TetrisType::Create(TCHAR const* group, TCHAR const* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
{
	TetrisTypeGroup* pTetrisTypeGroup = GetGroup(group);
	if (!pTetrisTypeGroup)
		pTetrisTypeGroup = CreateGroup();
	pTetrisTypeGroup->push_back(new TetrisType(group, name, penetrable, row, col, pData, dataCount, color));
}

void TetrisType::Delete(TetrisType* pTetrisType)
{
	GetGroup(pTetrisType->group)->remove(pTetrisType);
	delete pTetrisType;
}

void TetrisType::Clear()
{
	for (TetrisTypeLibrary::iterator itl = tetrisTypeLibrary.begin(); itl != tetrisTypeLibrary.end(); itl++)
	{
		for (TetrisTypeGroup::iterator itg = (*itl)->begin(); itg != (*itl)->end(); itg++)
		{
			delete *itg;
		}
		delete *itl;
	}
	tetrisTypeLibrary.clear();
}

TetrisTypeGroup* TetrisType::CreateGroup()
{
	TetrisTypeGroup* pTetrisTypeGroup = new TetrisTypeGroup;
	tetrisTypeLibrary.push_back(pTetrisTypeGroup);
	return pTetrisTypeGroup;
}

void TetrisType::DeleteGroup(TCHAR const* group)
{
	TetrisTypeGroup* pTetrisTypeGroup = GetGroup(group);
	tetrisTypeLibrary.remove(pTetrisTypeGroup);
	delete pTetrisTypeGroup;
}

bool TetrisType::ExsitGroup(TCHAR const* group)
{
	return nullptr != GetGroup(group);
}

TetrisTypeGroup* TetrisType::GetGroup(TCHAR const* group)
{
	for (TetrisTypeLibrary::iterator it = tetrisTypeLibrary.begin(); it != tetrisTypeLibrary.end(); it++)
	{
		TetrisType* pTetrisType = *((*it)->begin());
		if (0 == _tcscmp(pTetrisType->group, group))
			return *it;
	}
	return nullptr;
}

TetrisType* TetrisType::Random()
{
	TetrisTypeGroup* pTetrisTypeGroup = *next(tetrisTypeLibrary.begin(), Utility::Random(0, tetrisTypeLibrary.size() - 1));
	TetrisType* pTetrisType = *next(pTetrisTypeGroup->begin(), Utility::Random(0, pTetrisTypeGroup->size() - 1));
	return pTetrisType;
}

TetrisType* TetrisType::GetTetrisType(TCHAR const* group, TCHAR const* name)
{
	TetrisTypeGroup* pTetrisTypeGroup = GetGroup(group);
	if (!pTetrisTypeGroup)
		return nullptr;

	for (TetrisTypeGroup::iterator it = pTetrisTypeGroup->begin(); it != pTetrisTypeGroup->end(); it++)
	{
		if (0 == _tcscmp(name, (*it)->name))
			return *it;
	}
	return nullptr;
}


TetrisType::TetrisType(TCHAR const* const group, TCHAR const* const name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
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

bool TetrisType::Initialize(TCHAR const* group, TCHAR const* name, bool penetrable, int row, int col, char* pData, size_t dataCount, int color)
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

TetrisTypeLibrary TetrisType::tetrisTypeLibrary;