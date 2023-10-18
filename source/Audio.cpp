#include "Audio.h"
#include <assert.h>

void AudioEngine::Setup()
{
	this->result = FMOD::System_Create(&this->system);      // Create the main system object.
	assert(this->result == FMOD_OK);

	this->result = this->system->init(10, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	assert(this->result == FMOD_OK);
}

void AudioEngine::Destroy()
{
	for (int i = 0; i < 10; i++)
	{
		this->channel[i]->stop();
	}
	this->system->close();
	this->system->release();
}

void SoundComponent::Load(int EntityType)
{
	this->channelIndex = EntityType;

	switch (EntityType)
	{
	default:
		break;
	case MENU:
		//Load all menu sounds
		break;
	case BACKGROUND:
		//Load all background OSTs
		break;
	case AMBIENCE:
		//Load all ambience sounds
		break;
	case PLAYER:
		//Load all player sounds
		break;
	case IMP:
		//Load all imp sounds
		break;
	case SKELETON:
		//Load all skeleton sounds
		break;
	case EYE:
		//Load all eye sounds
		break;
	case HELLHOUND:
		//Load all hellhound sounds
		break;
	case BOSS:
		//Load all boss sounds
		break;
	}
}

void SoundComponent::Unload()
{
	
}
