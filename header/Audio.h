#pragma once
#include "fmod.hpp"
#include "MemLib\ML_Vector.hpp"

#define CHANNEL_LIMIT 2

enum Channel {Channel_Base, Channel_Extra};
enum UISFX {Button_Hover, Button_Press, Button_Start};
enum Music {Music_Title, Music_StageCalm, Music_StageCombat, Music_Shop, Music_Boss};
enum Ambience {Ambience_Lava};
enum Player {Player_Attack_Light, Player_Attack_Medium, Player_Attack_Heavy, Player_Attack_Charge, Player_Hurt, Player_Hurt2, Player_Hurt3, Player_Death, Player_Death2, Player_Dash};
enum PlayerVoice {Player_Footstep}; //Add all text later
enum Enemy {Enemy_Attack, Enemy_Hurt, Enemy_Death, Enemy_Special};
enum EnemyVoice {Enemy_Footstep, Enemy_Noise};
enum Boss {Boss_Attack_Hammer, Boss_Attack_Jump, Boss_Attack_Charge, Boss_Attack_Slam, Boss_Hurt};
enum BossVoice {Boss_Footstep}; //Add all text later
enum ImpVoice {}; //Add all text later

enum Type {MENU, MUSIC, AMBIENCE, PLAYER, IMP, SKELETON, EYE, HELLHOUND, BOSS};

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

struct AudioEngineComponent
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;
	ML_Vector<FMOD::Sound*> sounds;
	ML_Vector<FMOD::Channel*> channels;
	ML_Vector<int> freeChannels;
	ML_Vector<float> volumes;
	void Setup(int& ID);
	void AddChannel();
	void HandleSound();
	void HandleSpecificSound(bool& playSound, bool& stopSound, int& channelIndex, ML_Vector<int>& soundIndices, int& soundIndex);
	void Destroy();
};