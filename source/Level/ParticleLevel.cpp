#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "States\StateManager.h"
#include "Model.h"
#include "Levels\LevelHelper.h"

#include <random>

void LoadParticleLevel()
{
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.0f;
	StageSetupVariables stageVars;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 1;
	stageVars.scaleX = 1.0f;
	stageVars.scaleY = 1.0f;
	stageVars.scaleZ = 1.0f;
	stageVars.offsetY = -0.1f;

	srand(time(nullptr));

	CreatePlayer(0.0f, 0.0f, 0.0f, 3.0f, 1.0f, 20.0f, 10.0f, 1.0f, 0);

	EntityID stage = SetUpStage(stageVars);

	for (int i = 0; i < 100; i++)
	{
		EntityID particles = registry.CreateEntity();


		float random = ((float)rand()) / RAND_MAX * 40.f - 20.f;

		float firstRand = rand() & 1 ? 1 : -1;//rand() % 2 + (-1);
		float secondRand = rand() & 1 ? 1 : -1;

		TransformComponent transform;

		transform.mass = 1.f;
		transform.facingX = 1.0f; transform.facingY = 0.0f; transform.facingZ = 1.0f;
		transform.positionX = (random / (float)i) * (float)firstRand; transform.positionY = 5.0f; transform.positionZ = (random / (float)i) * (float)secondRand;
		transform.scaleX = 1.0f; transform.scaleY = 1.0f; transform.scaleZ = 1.0f;


		ComputeShaders cShad = SMOKE;

		if (i % 2 == 1)
			cShad = SMOKE;
		else
			cShad = NO_MOVEMENT;


		if (cShad == SMOKE)
		{
			registry.AddComponent<TransformComponent>(particles, transform);
			//Elliot: Adding a component this way is unsafe, a release is required if there already is a particleComponent
			//The solution: Find and release if it already exists
			ParticleComponent* particle = registry.GetComponent<ParticleComponent>(particles);
			if (particle != nullptr)
			{
				particle->Release();
			}
			registry.AddComponent<ParticleComponent>(particles, 5.0f, 10.f, 5.0f,
				0.f, 0.f, 0.f, 1000, cShad);

		}
		else
		{
			registry.AddComponent<TransformComponent>(particles, transform);
			//Elliot: Adding a component this way is unsafe, a release is required if there already is a particleComponent
			//The solution: Find and release if it already exists
			ParticleComponent* particle = registry.GetComponent<ParticleComponent>(particles);
			if (particle != nullptr)
			{
				particle->Release();
			}
			registry.AddComponent<ParticleComponent>(particles, 5.0f, 10.f, 5.0f,
				0.f, 0.f, 0.f, 2, VFX_PATTERN::FLAME);
		}
	
	}

	stateManager.stage = stage;
	SetInPlay(true);
}