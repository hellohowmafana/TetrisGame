#pragma once
#include "Configuration.hpp"

class Configurable
{
public:
	bool Initialize(Configuration* pConfiguration);
	bool Update(Configuration* pConfiguration);
	bool Deinitialize();

	bool IsInitialized() { return initialized; };

protected:
	virtual bool OnInitialize(Configuration* pConfiguration) { return true; };
	virtual bool OnUpdate(Configuration* pConfiguration) { return true; };
	virtual bool OnDeinitialize() { return true; };

protected:
	bool initialized;
};