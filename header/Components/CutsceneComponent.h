#pragma once
#include "EntityID.h"

//Mode to indicate what actions to make
enum CutsceneMode
{
	Cutscene_Nothing = 0b0,
	Transition_Position = 0b1, //Transition of position between two points, 
	Transition_LookAt = 0b10, //Transition of look at between two points

	Cutscene_Camera = 0b100,
	Cutscene_Linear = 0b1000,
	Cutscene_Accelerating = 0b10000,
	Cutscene_Decelerating = 0b100000,
	Cutscene_Character_Walk = 0b1000000,//Walk 0, auto facing
	Cutscene_Character_Fall = 0b10000000,//Idle 1
	Cutscene_Character_Idle = 0b100000000,//Idle 0
	Cutscene_Character_Attack = 0b1000000000,//Attack 0
	Cutscene_Character_BossLanding = 0b10000000000,//Walk 2
	Cutscene_Character_DogBrace = 0b100000000000,
	Cutscene_Character_DogBreath = 0b1000000000000,
};

//This component functions as arguments
//Use timed events for cutscene components
struct CutsceneComponent
{
	CutsceneMode mode = Cutscene_Nothing;
	float startPositionX, startPositionY, startPositionZ;
	float goalPositionX, goalPositionY, goalPositionZ;

	float startLookAtX, startLookAtY, startLookAtZ;
	float goalLookAtX, goalLookAtY, goalLookAtZ;
};

struct FallofComponent
{
	float fallofX, fallofZ;
	float portalX, portalZ;
};

void CutsceneSetPosition(EntityID& entity, float startX, float startY, float startZ, float goalX, float goalY, float goalZ);
void CutsceneSetLookAt(EntityID& entity, float startX, float startY, float startZ, float goalX, float goalY, float goalZ);

void CutsceneNPCIntro1NoText(EntityID& entity);