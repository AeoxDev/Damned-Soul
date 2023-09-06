#include "Audio.h"

Audio SetupAudio()
{
	Audio audio;
	
	audio.result = FMOD::System_Create(&audio.system);      // Create the main system object.
	if (audio.result != FMOD_OK)
	{
		printf("FMOD error! (%d)\n", audio.result);
		exit(-1);
	}

	audio.result = audio.system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (audio.result != FMOD_OK)
	{
		printf("FMOD error! (%d)\n", audio.result);
		exit(-1);
	}

	audio.system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &audio.pitchDSP);

	audio.system->createSound("../sunflower.mp3", FMOD_DEFAULT, 0, &audio.sound);
	audio.system->playSound(audio.sound, 0, false, &audio.channel);

	float pitchShift = 0.5f;
	audio.pitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchShift);

	audio.channel->addDSP(0, audio.pitchDSP);
	audio.channel->setPitch(0.1f);

	return audio;
}

void DestroyAudio(Audio& a)
{
	a.channel->stop();
	a.system->close();
	a.system->release();
	a.pitchDSP->release();
}
