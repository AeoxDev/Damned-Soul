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
#include "StaticHazardComponent.h"
//Event Components
#include "TimedEventComponent.h"
#include "SquashStretchComponent.h"
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
#include "Skynet\LuciferBehaviour.h"
#include "Skynet\FrozenBehaviour.h"

//Misc Combat Components
#include "DamageOverTimeComponent.h"
#include "ProjectileComponent.h"