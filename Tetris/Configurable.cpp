#include "Configurable.hpp"

bool Configurable::Initialize(Configuration* pConfiguration)
{
	if (initialized)
		return false;

	if(!OnInitialize(pConfiguration))
		return false;
	
	if(!Update(pConfiguration))
		return false;

	initialized = true;
	return true;
}

bool Configurable::Update(Configuration* pConfiguration)
{
	if (initialized)
		return false;

	if(!OnUpdate(pConfiguration))
		return false;

	return true;
}

bool Configurable::Deinitialize()
{
	if (!initialized)
		return false;
	
	if (!OnDeinitialize())
		return false;

	initialized = false;
	return true;
}
