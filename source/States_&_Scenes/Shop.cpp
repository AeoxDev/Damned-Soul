#include "States_&_Scenes\Shop.h"

void Shop::Update()
{
	if (i == 0)
	{
		std::cout << "I am in " << id << std::endl;
		i++;
	}
}

void Shop::WriteEntities(Registry& registry)
{
	std::cout << "Shop is empty" << std::endl;
}
