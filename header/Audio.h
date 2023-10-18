#pragma once
#include "fmod.hpp"
#include "MemLib\ML_Vector.hpp"

#define CHANNEL_LIMIT 2

const int MENU = 0;
const int MUSIC = 1;
const int AMBIENCE = 2;
const int PLAYER = 3;
const int IMP = 4;
const int SKELETON = 5;
const int EYE = 6;
const int HELLHOUND = 7;
const int BOSS = 8;

struct AudioEngineComponent
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;
	ML_Vector<FMOD::Sound*> sounds;
	ML_Vector<FMOD::Channel*> channels;
	ML_Vector<int> freeChannels;
	void Setup();
	void AddChannel();
	void HandleSound();
	void Destroy();
};

struct SoundComponent
{
	bool playSound[CHANNEL_LIMIT] = { false, false };
	bool stopSound[CHANNEL_LIMIT] = { false, false };
	int soundIndex[CHANNEL_LIMIT] = { 0, 0 };
	int channelIndex[CHANNEL_LIMIT] = { 0, 0 };
	//ML_Vector<FMOD::Sound*> sounds; If loading all sounds once doesn't work
	ML_Vector<int> (soundIndices)[CHANNEL_LIMIT];
	void Load(const int EntityType = 0);
	void Play(const int SoundIndex = 0, const int SelectedChannel = 0);
	void Stop(const int SelectedChannel = 0);
	void Unload();
};