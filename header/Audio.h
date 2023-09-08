#pragma once
#include "fmod.hpp"
#include "MemLib/MemLib.hpp"
#include <iostream>

struct Audio
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;

	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	FMOD::DSP* pitchDSP = nullptr;
};

Audio SetupAudio();

void DestroyAudio(Audio& a);