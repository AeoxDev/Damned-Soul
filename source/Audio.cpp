#include "Audio.h"
#include "Registry.h"
#include <assert.h>

void AudioEngineComponent::Setup(int& ID)
{
	this->result = FMOD::System_Create(&this->system);      // Create the main system object.
	assert(this->result == FMOD_OK);

	this->result = this->system->init(256, FMOD_INIT_NORMAL, 0);    // Initialize FMOD (The value 256 is the max amount of channels. If we have problems, increase this value)
	assert(this->result == FMOD_OK);

	FMOD::ChannelGroup* add = nullptr;
	this->groups.clear();
	this->groups.push_back(add); //Master
	this->groups.push_back(add); //SFX
	this->groups.push_back(add); //Music
	this->groups.push_back(add); //Voice
	this->groups.push_back(add); //Ambient

	system->getMasterChannelGroup(&this->groups[MASTER_GROUP]);
	system->createChannelGroup("SFX Group", &this->groups[SFX_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[SFX_GROUP]);
	system->createChannelGroup("Music Group", &this->groups[MUSIC_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[MUSIC_GROUP]);
	system->createChannelGroup("Voice Group", &this->groups[VOICE_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[VOICE_GROUP]);
	system->createChannelGroup("Ambient Group", &this->groups[AMBIENCE_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[AMBIENCE_GROUP]);

	//Load all sounds to use in the game
	FMOD::Sound* toAdd = nullptr;
	this->sounds.clear();
	for (int i = 0; i < 40; i++) //Change 1 to however many sounds you want to have in the game.
	{
		this->sounds.push_back(toAdd);
	}
	int i = 0;

	//Menu
	this->system->createSound("SFX/Menu/MouseHoverButton.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));
	this->system->createSound("SFX/Menu/MenuButtonPress.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));
	this->system->createSound("SFX/Menu/StartGameClick.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));

	//Music
	this->system->createSound("SFX/Music/TitleTheme.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/LavaCave.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/Shop.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/Ice.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/PatrickPlankton.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/CaveAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, AMBIENCE_GROUP));
	this->system->createSound("SFX/Music/LavaAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, AMBIENCE_GROUP));
	this->system->createSound("SFX/Music/BlizzardAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.15f, AMBIENCE_GROUP));

	//Player
	this->system->createSound("SFX/Player/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.15f, SFX_GROUP));
	this->system->createSound("SFX/Player/Dash.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.1f, SFX_GROUP));
	this->system->createSound("SFX/Player/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.1f, SFX_GROUP));
	this->system->createSound("SFX/Player/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Eye
	this->system->createSound("SFX/Enemy/Eye/Shoot.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Eye/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Eye/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Hellhound
	this->system->createSound("SFX/Enemy/Hellhound/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Inhale.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Flamethrower.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Skeleton
	this->system->createSound("SFX/Enemy/Skeleton/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Skeleton/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Skeleton/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Imp
	this->system->createSound("SFX/Enemy/Imp/AttackCharge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/AttackThrow.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Teleport.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Minotaur
	this->system->createSound("SFX/Enemy/Minotaur/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Jump.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Slam.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Miniboss
	this->system->createSound("SFX/Enemy/Miniboss/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Reassembling.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
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
				this->system->playSound(this->sounds[audio->soundIndices[audio->soundIndex[i]]], this->groups[this->volumes[audio->soundIndices[audio->soundIndex[i]]].group], false, &this->channels[audio->channelIndex[i]]); //Play the new sound
				this->channels[audio->channelIndex[i]]->setVolume(this->volumes[audio->soundIndices[audio->soundIndex[i]]].volume);
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
		this->system->playSound(this->sounds[soundIndices[soundIndex]], this->groups[this->volumes[soundIndices[soundIndex]].group], false, &this->channels[channelIndex]); //Play the new sound
		this->channels[channelIndex]->setVolume(this->volumes[soundIndices[soundIndex]].volume);
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
	for (int i = 4; i >= 0; i--)
	{
		this->groups[i]->stop();
		this->groups[i]->release();
		this->groups[i] = nullptr;
	}
	this->volumes.clear();
	this->groups.~ML_Vector();
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
		this->soundIndices.push_back((int)MENU1);
		this->soundIndices.push_back((int)MENU2);
		this->soundIndices.push_back((int)MENU3);
		break;
	case MUSIC:
		//Push back all indices for the music sounds into soundIndices
		this->soundIndices.push_back((int)MUSIC1);
		this->soundIndices.push_back((int)MUSIC2);
		this->soundIndices.push_back((int)MUSIC3);
		this->soundIndices.push_back((int)MUSIC4);
		this->soundIndices.push_back((int)MUSIC5);
		this->soundIndices.push_back((int)AMBIENCE1);
		this->soundIndices.push_back((int)AMBIENCE2);
		this->soundIndices.push_back((int)AMBIENCE3);
		this->soundIndices.push_back((int)MUSIC5); //Remove this later
		break;
	case PLAYER:
		//Push back all indices for the player sounds into soundIndices
		this->soundIndices.push_back((int)PLAYER1);
		this->soundIndices.push_back((int)PLAYER2);
		this->soundIndices.push_back((int)PLAYER3);
		this->soundIndices.push_back((int)PLAYER4);
		break;
	case EYE:
		//Push back all indices for the eye sounds into soundIndices
		this->soundIndices.push_back((int)EYE1);
		this->soundIndices.push_back((int)EYE2);
		this->soundIndices.push_back((int)EYE3);
		break;
	case HELLHOUND:
		//Push back all indices for the hellhound sounds into soundIndices
		this->soundIndices.push_back((int)HELLHOUND1);
		this->soundIndices.push_back((int)HELLHOUND2);
		this->soundIndices.push_back((int)HELLHOUND3);
		this->soundIndices.push_back((int)HELLHOUND4);
		this->soundIndices.push_back((int)HELLHOUND5);
		break;
	case SKELETON:
		//Push back all indices for the skeleton sounds into soundIndices
		this->soundIndices.push_back((int)SKELETON1);
		this->soundIndices.push_back((int)SKELETON2);
		this->soundIndices.push_back((int)SKELETON3);
		break;
	case IMP:
		//Push back all indices for the imp sounds into soundIndices
		this->soundIndices.push_back((int)IMP1);
		this->soundIndices.push_back((int)IMP2);
		this->soundIndices.push_back((int)IMP3);
		this->soundIndices.push_back((int)IMP4);
		this->soundIndices.push_back((int)IMP5);
		break;
	case MINOTAUR:
		//Push back all indices for the minotaur sounds into soundIndices
		this->soundIndices.push_back((int)MINOTAUR1);
		this->soundIndices.push_back((int)MINOTAUR2);
		this->soundIndices.push_back((int)MINOTAUR3);
		this->soundIndices.push_back((int)MINOTAUR4);
		this->soundIndices.push_back((int)MINOTAUR5);
		break;
	case MINIBOSS:
		//Push back all indices for the miniboss sounds into soundIndices
		this->soundIndices.push_back((int)MINIBOSS1);
		this->soundIndices.push_back((int)MINIBOSS2);
		this->soundIndices.push_back((int)MINIBOSS3);
		this->soundIndices.push_back((int)MINIBOSS4);
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
	if ((int)this->soundIndices.size() > SoundIndex)
	{
		this->soundIndex[SelectedChannel] = SoundIndex;
	}
	else
	{
		this->soundIndex[SelectedChannel] = (this->soundIndices.size() - 1);
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
			audioEngine->HandleSpecificSound(this->playSound[i], this->stopSound[i], this->channelIndex[i], this->soundIndices, this->soundIndex[i]);
			audioEngine->freeChannels.push_back(this->channelIndex[i]);
		}
	}
	this->soundIndices.~ML_Vector();
}
