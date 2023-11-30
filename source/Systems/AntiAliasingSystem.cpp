#include "Systems\Systems.h"
#include "AntiAlias.h"

bool AntiAliasingSystem::Update()
{
	AntiAlias::PrepareAntiAliasPass();
	//Dispatch(50, 29, 1);
	AntiAlias::FinishAntiAliasPass();
	return true;
}