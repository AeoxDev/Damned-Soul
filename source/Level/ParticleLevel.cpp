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

	EntityID stage = SetUpStage(stageVars);

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
		registry.AddComponent<ParticleComponent>(particles, (float)(rand() % (7 - 2 + 1) + 2), (float)(rand() % (20 - 5 + 1) + 5), (float)(rand() % (2 - 1 + 1) + 1),
			(float)(rand() % 40 + (-30)), (float)(rand() % 40 + (-30)), (float)(rand() % 40 + (-30)), rand() % (5000 - 1 + 1) + 1, cShad);
		
	}

	stateManager.stage = stage;
	SetInPlay(true);
}