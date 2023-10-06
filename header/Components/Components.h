#pragma once
#include "ComponentHelper.h"
#include "PointOfInterestComponent.h"

// Renderable Components
#include "ModelComponent.h"
#include "AnimationComponent.h"
// Special child belongs to both groups
#include "TransformComponent.h"
// Physics Components
#include "GeometryIndependentComponent.h"
#include "PlayerComponent.h"
#include "TimedEventComponent.h"

//GUI
#include "ButtonComponent.h"
#include "UIComponents/UIPlayerHealthComponent.h"
#include "UIComponents/UIPlayerSoulsComponent.h"
#include "UIComponents/UIPlayerRelicsComponent.h"
#include "UIComponents/UIBossHealthComponent.h"
#include "UIComponents/UIGameLevelComponent.h"

//GUI To Be Decapritated
#include "TextComponent.h"
#include "ImageComponent.h"
#include "ParticleComponent.h"

//Collision
#include "HitboxComponent.h"

#include "Skynet\SkeletonBehaviour.h"
#include "Skynet\HellhoundBehaviour.h"