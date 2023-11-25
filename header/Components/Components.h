#pragma once
#include "ComponentHelper.h"
#include "Audio.h"
//Camera
#include "PointOfInterestComponent.h"
#include "CutsceneComponent.h"

// Renderable Components
#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "LightComponent.h"
#include "GlowComponent.h"	// NOTE: This right? Should be rendered, so I guess?
// Special child belongs to both groups
#include "TransformComponent.h"
#include "FollowerComponent.h"
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
#include "Skynet\ImpBehaviour.h"
#include "Skynet\MinotaurBehaviour.h"
#include "Skynet\TempBossBehaviour.h"
#include "Skynet\LuciferBehaviour.h"
#include "Skynet\FrozenBehaviour.h"
#include "Skynet\ZacBehaviour.h"

//Misc Combat Components
#include "DebuffComponent.h"
#include "ProjectileComponent.h"