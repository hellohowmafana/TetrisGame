#pragma once
#include <string>
#include <vector>
using namespace std;

class IBinarySerializable
{
public:
	// pData 0 to obtain buffer size
	// pData may include argument of serialize
	virtual bool Save(char* pData, size_t& size) = 0;
	virtual bool Load(char* pData) = 0;

protected:
	void PutUchar(char*& pData, int val, size_t& totalSize);
	void TakeUchar(char*& pData, int& val);

	void PutUshort(char*& pData, int val, size_t& totalSize);
	void TakeUshort(char*& pData, int& val);

	void PutBool(char*& pData, bool val, size_t& totalSize);
	void TakeBool(char*& pData, bool& val);

	void PutWstring(char*& pData, wstring val, size_t& totalSize);
	void TakeWstring(char*& pData, wstring& val);

	void PutFloat(char*& pData, float val, size_t& totalSize);
	void TakeFloat(char*& pData, float& val);

	void PutUshortArray(char*& pData, vector<int>& vecVals, size_t& totalSize);
	void TakeUshortArray(char*& pData, vector<int>& vecVals);

	void PutFloatArray(char*& pData, vector<float>& vecVals, size_t& totalSize);
	void TakeFloatArray(char*& pData, vector<float>& vecVals);
};

