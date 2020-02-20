#pragma once

class IBinarySerializable
{
public:
	// pData nullptr to obtain buffer size
	virtual bool Save(char*& pData, unsigned int& size, char argument = 0) = 0;
	virtual bool Load(char*& pData, char argument = 0) = 0;
};

