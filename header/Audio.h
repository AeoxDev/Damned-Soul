#pragma once
#include "fmod.hpp"
#include "MemLib\ML_Vector.hpp"

#define CHANNEL_LIMIT 2

enum Channel {Channel_Base, Channel_Extra};
enum UISFX {Button_Hover, Button_Press, Button_Start};
enum Music {Music_Title, Music_Hot, Music_Shop, Music_Cold, Music_Boss};
enum Ambience {Ambience_Cave, Ambience_Lava, Ambience_Blizzard};
enum Player { Player_Attack, Player_Dash, Player_Hurt, Player_Death };//{Player_Attack_Light, Player_Attack_Medium, Player_Attack_Heavy, Player_Attack_Charge, Player_Hurt, Player_Hurt2, Player_Hurt3, Player_Death, Player_Death2, Player_Dash};
enum PlayerVoice {Player_Footstep}; //Add all text later
enum Eye {Eye_Attack, Eye_Hurt, Eye_Death};
enum Hellhound {Hellhound_Attack, Hellhound_Hurt, Hellhound_Inhale, Hellhound_Flame, Hellhound_Death};
enum Skeleton {Skeleton_Attack, Skeleton_Hurt, Skeleton_Death};
enum EnemyVoice {Enemy_Footstep, Enemy_Noise};
enum Boss {Boss_Attack_Hammer, Boss_Attack_Jump, Boss_Attack_Charge, Boss_Attack_Slam, Boss_Hurt};
enum BossVoice {Boss_Footstep}; //Add all text later
enum ImpVoice {}; //Add all text later

enum AllSounds{MENU1, MENU2, MENU3, MUSIC1, MUSIC2, MUSIC3, MUSIC4, MUSIC5, AMBIENCE1, AMBIENCE2, AMBIENCE3, PLAYER1, PLAYER2, PLAYER3, PLAYER4, EYE1, EYE2, EYE3,
				HELLHOUND1, HELLHOUND2, HELLHOUND3, HELLHOUND4, HELLHOUND5, SKELETON1, SKELETON2, SKELETON3};
enum Type {MENU, MUSIC, PLAYER, EYE, HELLHOUND, SKELETON, IMP, BOSS};

enum ChannelGroup {MASTER_GROUP, SFX_GROUP, MUSIC_GROUP, VOICE_GROUP};

struct SoundComponent
{
	bool playSound[CHANNEL_LIMIT] = { false, false };
	bool stopSound[CHANNEL_LIMIT] = { false, false };
	int soundIndex[CHANNEL_LIMIT] = { 0, 0 };
	int channelIndex[CHANNEL_LIMIT] = { 0, 0 };
	//ML_Vector<FMOD::Sound*> sounds; If loading all sounds once doesn't work
	ML_Vector<int>(soundIndices)[CHANNEL_LIMIT];
	void Load(const int EntityType = 0);
	void Play(const int SoundIndex = 0, const int SelectedChannel = 0);
	void Stop(const int SelectedChannel = 0);
	void Unload();
};

struct Volume
{
	float volume = 1.0f;
	int group = MASTER_GROUP;

	Volume(float vol = 1.0f, int gr = MASTER_GROUP)
	{
		this->volume = vol;
		this->group = gr;
	}
};

struct AudioEngineComponent
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;
	ML_Vector<FMOD::Sound*> sounds;
	ML_Vector<FMOD::Channel*> channels;
	ML_Vector<FMOD::ChannelGroup*> groups;
	ML_Vector<int> freeChannels;
	ML_Vector<Volume> volumes;
	void Setup(int& ID);
	void AddChannel();
	void HandleSound();
	void HandleSpecificSound(bool& playSound, bool& stopSound, int& channelIndex, ML_Vector<int>& soundIndices, int& soundIndex);
	void Destroy();
};