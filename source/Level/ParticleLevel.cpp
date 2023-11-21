#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "States\StateManager.h"
#include "Model.h"
#include <random>

void LoadParticleLevel()
{
	EntityID stage = registry.CreateEntity();

	srand(0);

	int loopAmount = rand() % (20 - 10 + 1) + 10;
	for (int i = 0; i < loopAmount; i++)
	{
		EntityID particles = registry.CreateEntity();

		TransformComponent transform;

		transform.mass = 1.f;
		transform.facingX = 1.0f; transform.facingY = 0.0f; transform.facingZ = 1.0f;
		transform.positionX = 0.0f; transform.positionY = 0.0f; transform.positionZ = 0.0f;
		transform.scaleX = 1.0f; transform.scaleY = 1.0f; transform.scaleZ = 1.0f;

		ComputeShaders cShad = SMOKE;

		if (i % 2 == 1)
			cShad = SMOKE;
		else
			cShad = FLAMETHROWER;

		registry.AddComponent<TransformComponent>(particles, transform);
		registry.AddComponent<ParticleComponent>(particles, rand() % (7 - 2 + 1) + 2, rand() % (20 - 5 + 1) + 5, rand() % (2 - 1 + 1) + 1, 
			rand() % 40 + (-30), rand() % 40 + (-30), rand() % 40 + (-30), rand() % (5000 - 1 + 1) + 1, cShad);
		
	}

	// Stage (Default)
	TransformComponent* stageTransform = registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");

	RenderGeometryIndependentCollision(stage);

	stateManager.stage = stage;
}