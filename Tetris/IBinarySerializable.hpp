#pragma once

class IBinarySerializable
{
public:
	// pData 0 to obtain buffer size
	virtual bool Save(char* pData, unsigned int& size, char argument = 0) = 0;
	virtual bool Load(char* pData) = 0;
};

