#pragma once
#include "ComponentHelper.h"
#include "PointOfInterestComponent.h"
#include "Audio.h"

// Renderable Components
#include "ModelComponent.h"
#include "AnimationComponent.h"
// Special child belongs to both groups
#include "TransformComponent.h"
#include "MouseComponent.h"
// Physics Components
#include "GeometryIndependentComponent.h"
#include "PlayerComponent.h"
#include "TimedEventComponent.h"

//GUI
#include "UIComponents/UIButtonComponent.h"
#include "UIComponents/UIHealthComponent.h"
#include "UIComponents/UIPlayerSoulsComponent.h"
#include "UIComponents/UIPlayerRelicsComponent.h"
#include "UIComponents/UIGameLevelComponent.h"

//GUI To Be Decapritated
#include "TextComponent.h"
#include "ImageComponent.h"
#include "ParticleComponent.h"

//Collision
#include "HitboxComponent.h"

//AI Behaviours
#include "Skynet\SkeletonBehaviour.h"
#include "Skynet\HellhoundBehaviour.h"
#include "Skynet/EyeBehaviour.h"