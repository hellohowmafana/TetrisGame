#include "IBinarySerializable.hpp"

void IBinarySerializable::PutUchar(char*& pData, int val, size_t& totalSize)
{
	size_t size = sizeof(unsigned char);
	if (pData)
	{
		memset(pData, (unsigned char)val, size);
		pData += size;
	}
	totalSize += size;
}

void IBinarySerializable::TakeUchar(char*& pData, int& val)
{
	val = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
}

void IBinarySerializable::PutUshort(char*& pData, int val, size_t& totalSize)
{
	size_t size = sizeof(unsigned short);
	if (pData)
	{
		memset(pData, (unsigned short)val, size);
		pData += size;
	}
	totalSize += size;
}

void IBinarySerializable::TakeUshort(char*& pData, int& val)
{
	val = *(unsigned short*)pData;
	pData += sizeof(unsigned short);
}

void IBinarySerializable::PutBool(char*& pData, bool val, size_t& totalSize)
{
	size_t size = sizeof(unsigned char);
	if (pData)
	{
		memset(pData, (unsigned char)(val ? 1 : 0), size);
		pData += size;
	}
	totalSize += size;
}

void IBinarySerializable::TakeBool(char*& pData, bool& val)
{
	val = *(unsigned char*)pData == 1;
	pData += sizeof(unsigned char);
}

void IBinarySerializable::PutWstring(char*& pData, wstring val, size_t& totalSize)
{
	size_t size = sizeof(wchar_t) * (val.size() + 1);
	if (pData)
	{
		wcscpy((wchar_t*)pData, val.c_str());
		pData += size;
	}
	totalSize += size;
}

void IBinarySerializable::TakeWstring(char*& pData, wstring& val)
{
	val = (wchar_t*)pData;
	pData += sizeof(wchar_t) * (val.size() + 1);
}

void IBinarySerializable::PutFloat(char*& pData, float val, size_t& totalSize)
{
	size_t size = sizeof(float);
	if (pData)
	{
		memcpy(pData, &val, size);
		pData += size;
	}
	totalSize += size;
}

void IBinarySerializable::TakeFloat(char*& pData, float& val)
{
	val = *(float*)pData;
	pData += sizeof(float);
}

void IBinarySerializable::PutUshortArray(char*& pData, vector<int>& vecVals, size_t& totalSize)
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
		memset(pData, (unsigned char)arraySize, sizeof(unsigned char));
		// array content
		for (vector<int>::iterator it = vecVals.begin(); it != vecVals.begin() + arraySize; it++)
		{
			memset(pData, (unsigned short)*it, sizeof(unsigned short));
			pData += sizeof(unsigned short);
		}
	}
	size_t size = (unsigned char)arraySize + sizeof(unsigned short) * arraySize;
	totalSize += size;
}

void IBinarySerializable::TakeUshortArray(char*& pData, vector<int>& vecVals)
{
	unsigned char arraySize = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
	for (size_t i = 0; i < arraySize; i++)
	{
		vecVals.push_back(*(unsigned short*)pData);
		pData += sizeof(unsigned short);
	}
}

void IBinarySerializable::PutFloatArray(char*& pData, vector<float>& vecVals, size_t& totalSize)
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
		memset(pData, (unsigned char)arraySize, sizeof(unsigned char));
		// array content
		for (vector<float>::iterator it = vecVals.begin(); it != vecVals.begin() + arraySize; it++)
		{
			memcpy(pData, &*it, sizeof(float));
			pData += sizeof(float);
		}
	}
	size_t size = sizeof(unsigned char) + sizeof(float) * arraySize;
	totalSize += size;
}

void IBinarySerializable::TakeFloatArray(char*& pData, vector<float>& vecVals)
{
	unsigned char arraySize = *(unsigned char*)pData;
	pData += sizeof(unsigned char);
	for (size_t i = 0; i < arraySize; i++)
	{
		vecVals.push_back(*(float*)pData);
		pData += sizeof(float);
	}
}