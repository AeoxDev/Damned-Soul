#include "Systems\Systems.h"
#include "Registry.h"
#include "Components.h"
#include "States\StateManager.h"
#include "DeltaTime.h"

float cooldown = 0.0f;

bool StageVoiceLineSystem::Update()
{
	//Handle voice lines to play occasionally on the stage
	SoundComponent* playerSounds = registry.GetComponent<SoundComponent>(stateManager.player);
	AudioEngineComponent* audioEngine = nullptr;
	for (auto entity : View<AudioEngineComponent>(registry))
	{
		audioEngine = registry.GetComponent<AudioEngineComponent>(entity);
	}

	if ((currentStates & InMainMenu) && (cooldown != 0)) //Reset cooldown if we are in main menu
	{
		cooldown = 0;
		if(audioEngine != nullptr) audioEngine->occasionalVoiceLinesPlayed.clear();
	}

	if ((playerSounds != nullptr) && (audioEngine != nullptr) && !(currentStates & InPause) && !(currentStates & InMainMenu)) //Check if the components exist and we are not in pause
	{
		if ((stateManager.activeLevel % 2) == 1)
		{
			cooldown += GetDeltaTime();
		}
		else
		{
			cooldown = 0.0f;
		}
		
		bool isPlaying = false;
		audioEngine->channels[playerSounds->channelIndex[Channel_Extra]]->isPlaying(&isPlaying); //Check if a line is already being played on this channel.

		if ((audioEngine->occasionalVoiceLinesPlayed.size() != 10) && (cooldown >= 15.0f) && (!isPlaying)) //Check to make sure that all available occasioanl sounds are not already played
		{
			int chance = rand() % 8192; //Rand controlling whether or not we should play a sound.
			if (chance == 0)
			{
				switch (stateManager.activeLevel)
				{
				case 1:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 0) //Check if no sounds have been played so far
					{
						playerSounds->Play(Player_WatchStep, Channel_Extra); //Cave 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 1)
					{
						playerSounds->Play(Player_PlaceSucks, Channel_Extra); //Cave 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 3:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 0)
					{
						playerSounds->Play(Player_WatchStep, Channel_Extra); //Cave 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 1)
					{
						playerSounds->Play(Player_PlaceSucks, Channel_Extra); //Cave 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 5:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 0)
					{
						playerSounds->Play(Player_WatchStep, Channel_Extra); //Cave 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 1)
					{
						playerSounds->Play(Player_PlaceSucks, Channel_Extra); //Cave 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 7:
					while (audioEngine->occasionalVoiceLinesPlayed.size() < 2)
					{
						audioEngine->occasionalVoiceLinesPlayed.push_back(0); //Fill up the list to make sure the next voice lines can be played
					}
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 2)
					{
						playerSounds->Play(Player_HotInHere, Channel_Extra); //Lava 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 3)
					{
						playerSounds->Play(Player_Hot, Channel_Extra); //Lava 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 4)
					{
						playerSounds->Play(Player_BurningUp, Channel_Extra); //Lava 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 9:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 2)
					{
						playerSounds->Play(Player_HotInHere, Channel_Extra); //Lava 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 3)
					{
						playerSounds->Play(Player_Hot, Channel_Extra); //Lava 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 4)
					{
						playerSounds->Play(Player_BurningUp, Channel_Extra); //Lava 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 11:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 2)
					{
						playerSounds->Play(Player_HotInHere, Channel_Extra); //Lava 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 3)
					{
						playerSounds->Play(Player_Hot, Channel_Extra); //Lava 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 4)
					{
						playerSounds->Play(Player_BurningUp, Channel_Extra); //Lava 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 13:
					while (audioEngine->occasionalVoiceLinesPlayed.size() < 5)
					{
						audioEngine->occasionalVoiceLinesPlayed.push_back(0); //Fill up the list to make sure the next voice lines can be played
					}
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 5)
					{
						playerSounds->Play(Player_IceInHell, Channel_Extra); //Ice 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 6)
					{
						playerSounds->Play(Player_Chills1, Channel_Extra); //Ice 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 7)
					{
						playerSounds->Play(Player_HotChocolate, Channel_Extra); //Ice 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 8)
					{
						playerSounds->Play(Player_Chills2, Channel_Extra); //Ice 4
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 9)
					{
						playerSounds->Play(Player_WasntSoBad, Channel_Extra); //Ice 5
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 15:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 5)
					{
						playerSounds->Play(Player_IceInHell, Channel_Extra); //Ice 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 6)
					{
						playerSounds->Play(Player_Chills1, Channel_Extra); //Ice 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 7)
					{
						playerSounds->Play(Player_HotChocolate, Channel_Extra); //Ice 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 8)
					{
						playerSounds->Play(Player_Chills2, Channel_Extra); //Ice 4
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 9)
					{
						playerSounds->Play(Player_WasntSoBad, Channel_Extra); //Ice 5
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				case 17:
					if (audioEngine->occasionalVoiceLinesPlayed.size() == 5)
					{
						playerSounds->Play(Player_IceInHell, Channel_Extra); //Ice 1
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 6)
					{
						playerSounds->Play(Player_Chills1, Channel_Extra); //Ice 2
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 7)
					{
						playerSounds->Play(Player_HotChocolate, Channel_Extra); //Ice 3
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 8)
					{
						playerSounds->Play(Player_Chills2, Channel_Extra); //Ice 4
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					else if (audioEngine->occasionalVoiceLinesPlayed.size() == 9)
					{
						playerSounds->Play(Player_WasntSoBad, Channel_Extra); //Ice 5
						cooldown = 0;
						audioEngine->occasionalVoiceLinesPlayed.push_back(0);
					}
					break;
				}
			}
		}
	}

	return true;
}