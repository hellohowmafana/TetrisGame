#pragma once
#include <string>
#include <vector>
using namespace std;

class BinarySerializer
{
public:
	void PutUchar(char*& pData, int val, unsigned int& totalSize);
	void TakeUchar(char*& pData, int& val);

	void PutUshort(char*& pData, int val, unsigned int& totalSize);
	void TakeUshort(char*& pData, int& val);

	void PutBool(char*& pData, bool val, unsigned int& totalSize);
	void TakeBool(char*& pData, bool& val);

	void PutWstring(char*& pData, wstring val, unsigned int& totalSize);
	void TakeWstring(char*& pData, wstring& val);

	void PutFloat(char*& pData, float val, unsigned int& totalSize);
	void TakeFloat(char*& pData, float& val);

	void PutUshortArray(char*& pData, vector<int>& vecVals, unsigned int& totalSize);
	void TakeUshortArray(char*& pData, vector<int>& vecVals);

	void PutFloatArray(char*& pData, vector<float>& vecVals, unsigned int& totalSize);
	void TakeFloatArray(char*& pData, vector<float>& vecVals);
};

