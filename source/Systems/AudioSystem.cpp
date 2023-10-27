#include "Systems\Systems.h"
#include "Registry.h"
#include "Components.h"

bool AudioSystem::Update()
{
	//Handle Sounds
	for (auto entity : View<AudioEngineComponent>(registry))
	{
		AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);

		//Handle the sound LoL
		audioEngine->HandleSound();
	}

	return true;
}