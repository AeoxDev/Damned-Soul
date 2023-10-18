#pragma once
#include "fmod.hpp"
#include "MemLib\ML_Vector.hpp"

const int MENU = 0;
const int BACKGROUND = 1;
const int AMBIENCE = 2;
const int PLAYER = 3;
const int IMP = 4;
const int SKELETON = 5;
const int EYE = 6;
const int HELLHOUND = 7;
const int BOSS = 8;

struct AudioEngine
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;
	FMOD::Channel* channel[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	void Setup();
	void Destroy();
};

struct SoundComponent
{
	int channelIndex = 0;
	ML_Vector<FMOD::Sound*> sounds;
	void Load(int EntityType);
	void Unload();
};