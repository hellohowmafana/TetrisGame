#include "BinarySerializer.h"

void BinarySerializer::PutUchar(char*& pData, int val, unsigned int& totalSize)
{
	unsigned int size = sizeof(unsigned char);
	if (pData)
	{
		memcpy(pData, &val, size);
		pData += size;
	}
	totalSize += size;
}

void BinarySerializer::TakeUchar(char*& pData, int& val)
{
	val = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
}

void BinarySerializer::PutUshort(char*& pData, int val, unsigned int& totalSize)
{
	unsigned int size = sizeof(unsigned short);
	if (pData)
	{
		memcpy(pData, &val, size);
		pData += size;
	}
	totalSize += size;
}

void BinarySerializer::TakeUshort(char*& pData, int& val)
{
	val = *(unsigned short*)pData;
	pData += sizeof(unsigned short);
}

void BinarySerializer::PutBool(char*& pData, bool val, unsigned int& totalSize)
{
	unsigned int size = sizeof(unsigned char);
	if (pData)
	{
		memcpy(pData, &val, size);
		pData += size;
	}
	totalSize += size;
}

void BinarySerializer::TakeBool(char*& pData, bool& val)
{
	val = *(unsigned char*)pData == 1;
	pData += sizeof(unsigned char);
}

void BinarySerializer::PutWstring(char*& pData, wstring val, unsigned int& totalSize)
{
	unsigned int size = static_cast<unsigned int>(sizeof(wchar_t) * (val.size() + 1));
	if (pData)
	{
		wcscpy((wchar_t*)pData, val.c_str());
		pData += size;
	}
	totalSize += size;
}

void BinarySerializer::TakeWstring(char*& pData, wstring& val)
{
	val = (wchar_t*)pData;
	pData += sizeof(wchar_t) * (val.size() + 1);
}

void BinarySerializer::PutFloat(char*& pData, float val, unsigned int& totalSize)
{
	unsigned int size = sizeof(float);
	if (pData)
	{
		memcpy(pData, &val, size);
		pData += size;
	}
	totalSize += size;
}

void BinarySerializer::TakeFloat(char*& pData, float& val)
{
	val = *(float*)pData;
	pData += sizeof(float);
}

void BinarySerializer::PutUshortArray(char*& pData, vector<int>& vecVals, unsigned int& totalSize)
{
	if (0 == vecVals.size())
		return;

	// last same elements stored as one single, obtain needed array size
	size_t arraySize = 1;
	for (vector<int>::iterator it = vecVals.end() - 1; it != vecVals.begin(); it--)
	{
		if (vecVals.back() != *it)
		{
			arraySize = it + 1 - vecVals.begin();
			break;
		}
	}

	if (pData)
	{
		// array size
		memcpy(pData, &arraySize, sizeof(unsigned char));
		// array content
		for (vector<int>::iterator it = vecVals.begin(); it != vecVals.begin() + arraySize; it++)
		{
			memcpy(pData, &*it, sizeof(unsigned short));
			pData += sizeof(unsigned short);
		}
	}
	unsigned int size = static_cast<unsigned int>((unsigned char)arraySize + sizeof(unsigned short) * arraySize);
	totalSize += size;
}

void BinarySerializer::TakeUshortArray(char*& pData, vector<int>& vecVals)
{
	unsigned char arraySize = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
	for (size_t i = 0; i < arraySize; i++)
	{
		vecVals.push_back(*(unsigned short*)pData);
		pData += sizeof(unsigned short);
	}
}

void BinarySerializer::PutFloatArray(char*& pData, vector<float>& vecVals, unsigned int& totalSize)
{
	if (0 == vecVals.size())
		return;

	// last same elements stored as one single, obtain needed array size
	size_t arraySize = 1;
	for (vector<float>::iterator it = vecVals.end() - 1; it != vecVals.begin(); it--)
	{
		if (vecVals.back() != *it)
		{
			arraySize = it + 1 - vecVals.begin();
			break;
		}
	}

	if (pData)
	{
		// array size
		memcpy(pData, &arraySize, sizeof(unsigned char));
		// array content
		for (vector<float>::iterator it = vecVals.begin(); it != vecVals.begin() + arraySize; it++)
		{
			memcpy(pData, &*it, sizeof(float));
			pData += sizeof(float);
		}
	}
	unsigned int size = static_cast<unsigned int>(sizeof(unsigned char) + sizeof(float) * arraySize);
	totalSize += size;
}

void BinarySerializer::TakeFloatArray(char*& pData, vector<float>& vecVals)
{
	unsigned char arraySize = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
	for (size_t i = 0; i < arraySize; i++)
	{
		vecVals.push_back(*(float*)pData);
		pData += sizeof(float);
	}
}