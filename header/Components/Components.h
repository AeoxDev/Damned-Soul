#pragma once
#include "ComponentHelper.h"
#include "PointOfInterestComponent.h"
#include "Audio.h"

// Renderable Components
#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "LightComponent.h"
// Special child belongs to both groups
#include "TransformComponent.h"
#include "MouseComponent.h"
// Physics Components
#include "GeometryIndependentComponent.h"
#include "TimedEventComponent.h"

//To Be Decapritated
#include "ParticleComponent.h"

//Collision
#include "HitboxComponent.h"
#include "KnockBackComponent.h"

//AI Behaviours
#include "Skynet\SkeletonBehaviour.h"
#include "Skynet\HellhoundBehaviour.h"
#include "Skynet\EyeBehaviour.h"
#include "Skynet\TempBossBehaviour.h"

//Misc Combat Components
#include "DamageOverTimeComponent.h"