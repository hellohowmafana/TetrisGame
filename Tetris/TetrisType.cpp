#include "TetrisType.h"
#include "Utility.h"

void TetrisType::Create(TCHAR const* group, TCHAR const* name,
	bool penetrable, bool twoRotation, bool clockwiseRotation,
	int horizontalCenterOffset,
	int row, int col, char* pData, size_t dataCount, int color)
{
	TetrisTypeGroup* pTetrisTypeGroup = GetGroup(group);
	if (!pTetrisTypeGroup)
		pTetrisTypeGroup = CreateGroup(group);
	pTetrisTypeGroup->push_back(new TetrisType(group, name, penetrable, twoRotation, clockwiseRotation, horizontalCenterOffset, row, col, pData, dataCount, color));
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
		for (TetrisTypeGroup::iterator itg = itl->second->begin(); itg != itl->second->end(); itg++)
		{
			delete *itg;
		}
		delete itl->second;
	}
	tetrisTypeLibrary.clear();
}

TetrisTypeGroup* TetrisType::CreateGroup(TCHAR const* group)
{
	if (ExsitGroup(group))
		return nullptr;
	TetrisTypeGroup* pTetrisTypeGroup = new TetrisTypeGroup;
	tetrisTypeLibrary.emplace(group, pTetrisTypeGroup);
	return pTetrisTypeGroup;
}

void TetrisType::DeleteGroup(TCHAR const* group)
{
	TetrisTypeGroup* pTetrisTypeGroup = GetGroup(group);
	delete pTetrisTypeGroup;
	tetrisTypeLibrary.erase(group);
}

bool TetrisType::ExsitGroup(TCHAR const* group)
{
	return nullptr != GetGroup(group);
}

TetrisTypeGroup* TetrisType::GetGroup(TCHAR const* group)
{
	if (tetrisTypeLibrary.end() == tetrisTypeLibrary.find(group))
		return nullptr;
	else
		return tetrisTypeLibrary[group];
}

int TetrisType::GetTetrisTypesCount()
{
	int count = 0;
	for (TetrisTypeLibrary::iterator it = tetrisTypeLibrary.begin(); it != tetrisTypeLibrary.end(); it++)
	{
		count += it->second->size();
	}
	return count;
}

int TetrisType::GetTetrisTypesCount(TCHAR const* group)
{
	if(!ExsitGroup(group))
		return 0;
	return tetrisTypeLibrary[group]->size();
}

int TetrisType::GetRandomColor(TCHAR const* group)
{
	return Utility::Random(0, GetTetrisTypesCount(group) - 1);
}

TetrisType* TetrisType::Random()
{
	TetrisTypeGroup* pTetrisTypeGroup = next(tetrisTypeLibrary.begin(), Utility::Random(0, tetrisTypeLibrary.size() - 1))->second;
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

TetrisType::TetrisType(TCHAR const* const group, TCHAR const* const name,
	bool penetrable, bool twoRotation, bool clockwiseRotation,
	int horizontalCenterOffset,
	int row, int col, char* pData, size_t dataCount, int color)
:group(NULL), name(NULL), pData(NULL)
{
	Initialize(group, name, penetrable, twoRotation, clockwiseRotation, horizontalCenterOffset, row, col, pData, dataCount, color);
}

TetrisType::~TetrisType()
{
	delete[] group;
	delete[] name;
	delete[] pData;
}

bool TetrisType::Initialize(TCHAR const* group, TCHAR const* name,
	bool penetrable, bool twoRotation, bool clockwiseRotation,
	int horizontalCenterOffset,
	int row, int col, char* pData, size_t dataCount, int color)
{
	size_t len;
	
	len = _tcslen(group);
	this->group = new TCHAR[len];
	_tcscpy(this->group, group);
	
	len = _tcslen(name);
	this->name = new TCHAR[len];
	_tcscpy(this->name, name);

	this->penetrable = penetrable;
	this->twoRotation = twoRotation;
	this->clockwiseRotation = clockwiseRotation;
	this->horizontalCenterOffset = horizontalCenterOffset;
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

const TCHAR* TetrisType::classic = _T("classic");

TetrisTypeLibrary TetrisType::tetrisTypeLibrary;