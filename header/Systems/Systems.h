#pragma once

class System
{
public:
	virtual bool Update() = 0;
};


class RenderSystem : public System
{
	bool Update();
};
class ButtonSystem : public System
{
	bool Update();
};
class ControllerSystem : public System
{
	bool Update();
};
class GeometryIndependentSystem : public System
{
	bool Update();
};

class UIRenderSystem : public System
{
	bool Update();
};
class UIHealthSystem : public System
{
	bool Update();
};
class UIPlayerSoulsSystem : public System
{
	bool Update();
};
class UIPlayerRelicsSystem : public System
{
	bool Update();
};
class UIGameLevelSystem : public System
{
	bool Update();
};

class PointOfInterestSystem : public System
{
	bool Update();
};
class TransformSystem : public System
{
	bool Update();
};
class ParticleSystemGPU : public System
{
	bool Update();
};
class ParticleSystemCPU : public System
{
	bool Update();
};

class SkeletonBehaviourSystem : public System
{
	bool Update();
};

class HellhoundBehaviourSystem : public System
{
	bool Update();
};

class CollisionSystem : public System
{
	bool Update();
};

class EventSystem : public System
{
	bool Update();
};

//System implementation that'll pretty much need to be worked into the scene class and not fuck up the rest of the project
//Order of operations in the main update loop is essentially:
//1. Do general window events
//2. Clear
//3. Update systems (includes Draw-calls)
//4. Display
//And repeat
/*
//Funny system update loop
for (auto it = systems.begin(); it != systems.end(); it++)
{
	if ((*it)->Update(registry, dt))
	{
		//Update does whatever it does, and IF it returns true afterwards we delete the system
		delete (*it);
		it = systems.erase(it);
		if (systems.size() == 0) //No more update if we've officially run out of systems to update
			return;
	}
}

#pragma once

#include "EntityFramework.h"

//Global window ehe
sf::String defaultTitle = "Active Relic: ";
sf::RenderWindow window(sf::VideoMode(800, 600), defaultTitle);

class System
{
public:
	virtual bool Update(Registry& registry, float dt) = 0;
};

class PlayerSystem : public System
{
public:
	bool Update(Registry& registry, float dt)
	{
		auto view = View<PlayerComponent, GraphicsComponent>(registry);
		for (auto entity : view)
		{
			auto playerComp = registry.GetComponent<PlayerComponent>(entity);
			auto graphicsComp = registry.GetComponent<GraphicsComponent>(entity);

			Move(playerComp, graphicsComp, dt);
		}

		return false;
	}

private:
	void Move(PlayerComponent* playerComp, GraphicsComponent* graphicsComp, float dt)
	{
		float speed = playerComp->speed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			playerComp->AddRelic<SpeedRelic>();
			window.setTitle(defaultTitle + "Speed");
		}
		else
		{
			playerComp->RemoveRelic<SpeedRelic>();
			window.setTitle(defaultTitle + "None");
		}

		if (playerComp->GetRelic<SpeedRelic>()) //If the player has the speed relic they move at double the speed
			speed *= 2.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			graphicsComp->body.move(speed * dt, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			graphicsComp->body.move(-speed * dt, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			graphicsComp->body.move(0.0f, -speed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			graphicsComp->body.move(0.0f, speed * dt);
		}
	}
};

class GraphicsSystem : public System
{
public:
	GraphicsSystem(sf::RenderWindow* window) : window(window) {}

	bool Update(Registry& registry, float dt)
	{
		auto view = View<GraphicsComponent>(registry);
		for (auto entity : view)
		{
			auto comp = registry.GetComponent<GraphicsComponent>(entity);
			window->draw(comp->body);
		}

		return false;
	}
private:
	sf::RenderWindow* window;
};
*/