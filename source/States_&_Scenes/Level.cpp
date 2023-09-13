#include "States_&_Scenes\Level.h"

Level::Level(std::string id, Registry& registry, std::vector<std::string> entityList)
	:id(id), entityList(entityList)
{
	for (std::string entityId : entityList)
	{
		auto entity = registry.CreateEntity();
		if (entityId == "Imp")
		{
			registry.AddComponent<ImpComponent>(entity, 2);
		}
		else if (entityId == "Skeleton")
		{
			registry.AddComponent<SkeletonComponent>(entity, 8);
		}
		else if (entityId == "Demon")
		{
			registry.AddComponent<DemonComponent>(entity, 12);
		}
	}
}

void Level::WriteEntities(Registry& registry)
{
	auto impView = View<ImpComponent>(registry);
	auto playerView = View<PlayerComponent>(registry);
	auto skeletonView = View<SkeletonComponent>(registry);
	auto demonView = View<DemonComponent>(registry);

	int i = 0;

	for (auto entity : playerView)
	{
		auto comp = registry.GetComponent<PlayerComponent>(entity);
		if (comp)
			std::cout << "Entity: " << entity.index << " Player Hp: " << comp->hp << std::endl;
	}

	std::cout << std::endl;

	for (auto entity : impView)
	{
		auto comp = registry.GetComponent<ImpComponent>(entity);
		if (comp)
		{
			i++;
			std::cout << "Entity: " << entity.index << " Imp Hp: " << comp->hp << std::endl;
		}
	}

	if (i > 0)
		std::cout << std::endl;
	i = 0;

	for (auto entity : skeletonView)
	{
		auto comp = registry.GetComponent<SkeletonComponent>(entity);
		if (comp)
		{
			i++;
			std::cout << "Entity: " << entity.index << " Skeleton Hp: " << comp->hp << std::endl;
		}
	}

	if (i > 0)
		std::cout << std::endl;
	i = 0;

	for (auto entity : demonView)
	{
		auto comp = registry.GetComponent<DemonComponent>(entity);
		if (comp)
		{
			i++;
			std::cout << "Entity: " << entity.index << " Demon Hp: " << comp->hp << std::endl;
		}
	}

	if (i > 0)
		std::cout << std::endl;

}

void Level::Update()
{
	if (i == 0)
	{
		std::cout << "I am in " << id << std::endl;
		i++;
	}
}

void Level::ResetEntities(Registry& registry)
{
	auto impView = View<ImpComponent>(registry);
	auto skeletonView = View<SkeletonComponent>(registry);
	auto demonView = View<DemonComponent>(registry);

	for (auto entity : impView)
		registry.RemoveComponent<ImpComponent>(entity);

	for (auto entity : skeletonView)
		registry.RemoveComponent<SkeletonComponent>(entity);

	for (auto entity : demonView)
		registry.RemoveComponent<DemonComponent>(entity);
}
