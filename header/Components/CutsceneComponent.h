#pragma once
#include "EntityID.h"

//Mode to indicate what actions to make
enum CutsceneMode
{
	Transition_Position = 0b1, //Transition of position between two points, 
	Transition_LookAt = 0b01, //Transition of look at between two points

	Cutscene_Linear = 0b001
};

//This component functions as arguments
//Use timed events for cutscene components
struct CutsceneComponent
{
	CutsceneMode mode;
	float startPositionX, startPositionY, startPositionZ;
	float goalPositionX, goalPositionY, goalPositionZ;

	float startLookAtX, startLookAtY, startLookAtZ;
	float goalLookAtX, goalLookAtY, goalLookAtZ;
};

void CutsceneNPCIntro1NoText(EntityID& entity);