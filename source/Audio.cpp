#include "Audio.h"
#include "Registry.h"
#include <assert.h>

void AudioEngineComponent::Setup(int& ID)
{
	this->result = FMOD::System_Create(&this->system);      // Create the main system object.
	assert(this->result == FMOD_OK);

	this->result = this->system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD (The value 32 is the max amount of channels. If we have problems, increase this value)
	assert(this->result == FMOD_OK);

	//Load all sounds to use in the game
	FMOD::Sound* toAdd;
	this->sounds.clear();
	for (int i = 0; i < 6; i++) //Change 1 to however many sounds you want to have in the game.
	{
		this->sounds.push_back(toAdd);
	}
	this->system->createSound("SFX/Menu/MouseHoverButton.mp3", FMOD_DEFAULT, 0, &this->sounds[0]);
	this->volumes.push_back(1.0f);
	this->system->createSound("SFX/Menu/MenuButtonPress.mp3", FMOD_DEFAULT, 0, &this->sounds[1]);
	this->volumes.push_back(1.0f);
	this->system->createSound("SFX/Menu/StartGameClick.mp3", FMOD_DEFAULT, 0, &this->sounds[2]);
	this->volumes.push_back(1.0f);
	this->system->createSound("SFX/Music/TitleTheme.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[3]);
	this->volumes.push_back(0.5f);
	this->system->createSound("SFX/Music/StageCalm.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[4]);
	this->volumes.push_back(0.5f);
	this->system->createSound("SFX/Music/StageCombat.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[5]);
	this->volumes.push_back(0.0f);
}

void AudioEngineComponent::HandleSound()
{
	for (auto entity : View<SoundComponent>(registry)) //Access an entity
	{
		SoundComponent* audio = registry.GetComponent<SoundComponent>(entity);
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			if (audio->playSound[i]) //Check if the entity wants to play a sound
			{
				//Play the sound
				if (this->channels[audio->channelIndex[i]] != nullptr)
				{
					this->channels[audio->channelIndex[i]]->stop(); //Stop the previous sound
					this->channels[audio->channelIndex[i]] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
				}
				this->system->playSound(this->sounds[audio->soundIndices[i][audio->soundIndex[i]]], 0, false, &this->channels[audio->channelIndex[i]]); //Play the new sound
				this->channels[audio->channelIndex[i]]->setVolume(this->volumes[audio->soundIndices[i][audio->soundIndex[i]]]);
				audio->playSound[i] = false;
			}
			else if (audio->stopSound[i])
			{
				//Stop the sound
				this->channels[audio->channelIndex[i]]->stop(); //Stop the previous sound
				this->channels[audio->channelIndex[i]] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
				audio->stopSound[i] = false;
			}
		}
	}
}

void AudioEngineComponent::AddChannel()
{
	//Create a new channel and push it back into the vector
	FMOD::Channel* toAdd = nullptr;
	this->channels.push_back(toAdd);
}

void AudioEngineComponent::HandleSpecificSound(bool& playSound, bool& stopSound, int& channelIndex, ML_Vector<int>& soundIndices, int& soundIndex)
{
	if (playSound) //Check if the entity wants to play a sound
	{
		//Play the sound
		if (this->channels[channelIndex] != nullptr)
		{
			this->channels[channelIndex]->stop(); //Stop the previous sound
			this->channels[channelIndex] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
		}
		this->system->playSound(this->sounds[soundIndices[soundIndex]], 0, false, &this->channels[channelIndex]); //Play the new sound
		this->channels[channelIndex]->setVolume(this->volumes[soundIndices[soundIndex]]);
		playSound = false;
	}
	else if (stopSound)
	{
		//Stop the sound
		this->channels[channelIndex]->stop(); //Stop the previous sound
		this->channels[channelIndex] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
		stopSound = false;
	}
}

void AudioEngineComponent::Destroy()
{
	//Stop all sounds and release everything
	for (int i = 0; i < (int)this->channels.size(); i++)
	{
		if (this->channels[i] != nullptr)
		{
			this->channels[i]->stop();
			this->channels[i] = nullptr;
		}
	}
	for (int i = 0; i < (int)this->sounds.size(); i++)
	{
		this->sounds[i]->release();
	}
	this->sounds.~ML_Vector();
	this->channels.~ML_Vector();
	this->freeChannels.~ML_Vector();
	this->volumes.~ML_Vector();
	this->system->close();
	this->system->release();
}

void SoundComponent::Load(const int EntityType)
{
	for (auto entity : View<AudioEngineComponent>(registry)) //Access an entity
	{
		AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);

		//Check if there is a free channel that already exists
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			if ((int)audioEngine->freeChannels.size() > 0)
			{
				this->channelIndex[i] = audioEngine->freeChannels[(audioEngine->freeChannels.size() - 1)];
				audioEngine->freeChannels.pop_back();
			}
			else //If not enough space exists, create a new one
			{
				audioEngine->AddChannel();
				this->channelIndex[i] = (audioEngine->channels.size() - 1);
			}
		}
	}

	switch (EntityType)
	{
	default:
		break;
	case MENU:
		//Push back all indices for the menu sounds into soundIndices
		this->soundIndices[0].push_back(0);
		this->soundIndices[0].push_back(1);
		this->soundIndices[0].push_back(2);
		break;
	case MUSIC:
		//Push back all indices for the music sounds into soundIndices
		this->soundIndices[0].push_back(3);
		this->soundIndices[0].push_back(4);
		this->soundIndices[0].push_back(5);
		this->soundIndices[1].push_back(3);
		this->soundIndices[1].push_back(4);
		this->soundIndices[1].push_back(5);
		break;
	case AMBIENCE:
		//Push back all indices for the ambience sounds into soundIndices
		break;
	case PLAYER:
		//Push back all indices for the player sounds into soundIndices
		break;
	case IMP:
		//Push back all indices for the imp sounds into soundIndices
		break;
	case SKELETON:
		//Push back all indices for the skeleton sounds into soundIndices
		break;
	case EYE:
		//Push back all indices for the eye sounds into soundIndices
		break;
	case HELLHOUND:
		//Push back all indices for the hellhound sounds into soundIndices
		break;
	case BOSS:
		//Push back all indices for the boss sounds into soundIndices
		break;
	}
}

void SoundComponent::Play(const int SoundIndex, const int SelectedChannel)
{
	//Set playSound to true and the selected sound index, make sure the index is within the valid range as well
	this->playSound[SelectedChannel] = true;
	if ((int)this->soundIndices[SelectedChannel].size() > SoundIndex)
	{
		this->soundIndex[SelectedChannel] = SoundIndex;
	}
	else
	{
		this->soundIndex[SelectedChannel] = (this->soundIndices[SelectedChannel].size() - 1);
	}
}

void SoundComponent::Stop(const int SelectedChannel)
{
	//Set stopSound to true
	this->stopSound[SelectedChannel] = true;
}

void SoundComponent::Unload()
{
	for (auto entity : View<AudioEngineComponent>(registry)) //Access an entity
	{
		AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);

		//Play the audio before removing it and set the index for the channel as free.
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			audioEngine->HandleSpecificSound(this->playSound[i], this->stopSound[i], this->channelIndex[i], this->soundIndices[i], this->soundIndex[i]);
			audioEngine->freeChannels.push_back(this->channelIndex[i]);
		}
	}
	for (int i = 0; i < CHANNEL_LIMIT; i++)
	{
		this->soundIndices[i].~ML_Vector();
	}
}
