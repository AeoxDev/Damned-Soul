#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include <cmath> //sin
#include <assert.h>
#include <string>
#include "States\StateManager.h"
#include "UIComponents.h"
#include "UIRenderer.h"
#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

void DestroyEntity(EntityID& entity, const int& index)
{
	registry.DestroyEntity(entity);
	RedrawUI();
}

void UIFollowTransform(EntityID& entity, const int& index)
{
	//Calc position to screenpos
	//Then get the position of the transformComponent. Take the transformComponent to the screenspace.
	TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(entity);
	assert(transformComponent != nullptr);//The mouse requires a transformComponent in order to update its direction!
	XMVECTOR toScreenSpace = XMVECTOR{ transformComponent->positionX,transformComponent->positionY, transformComponent->positionZ };
	toScreenSpace = XMVector3Transform(toScreenSpace, Camera::GetView());
	toScreenSpace = XMVector3Transform(toScreenSpace, Camera::GetPerspective());
	XMFLOAT4 toFloat;
	XMStoreFloat4(&toFloat, toScreenSpace);
	float scalar = 0.07f * sinf(1.67f * (GetTimedEventElapsedTime(entity, index) / GetTimedEventTotalTime(entity, index)));
	if (GetTimedEventCondition(entity, index) == 1)
	{
		scalar *= 0.3f;
	}
	float dir = 0.3f * sinf((float)entity.index);
	float positionX = dir * scalar + toFloat.x / toFloat.w;
	float positionY = scalar + 0.15f + (toFloat.y / toFloat.w);
	
	UIComponent* uiElement2 = registry.GetComponent<UIComponent>(entity);
	uiElement2->m_BaseText.baseUI.SetPosition(DSFLOAT2(positionX, positionY));
	//Change font color
	RedrawUI();
}

void CreateDamageNumber(EntityID& entity, const int& index)
{
	//Create a damagenumber text depending on damage done:
	unsigned damage = GetTimedEventCondition(entity, index);
	float damageSize = 10.0f * damage *damage / 3600.0f;//Double at 40 damage (reach 10)
	if (damageSize > 80.0f)
	{
		damageSize = 80.0f;
	}
	float time = GetTimedEventTotalTime(entity, index);
	//Create the damage number
	
	//Calc position to screenpos
	//Then get the position of the transformComponent. Take the transformComponent to the screenspace.
	TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(entity);
	assert(transformComponent != nullptr);//The mouse requires a transformComponent in order to update its direction!
	XMVECTOR toScreenSpace = XMVECTOR{ transformComponent->positionX,transformComponent->positionY, transformComponent->positionZ };
	toScreenSpace = XMVector3Transform(toScreenSpace, Camera::GetView());
	toScreenSpace = XMVector3Transform(toScreenSpace, Camera::GetPerspective());
	XMFLOAT4 toFloat;
	XMStoreFloat4(&toFloat, toScreenSpace);

	float positionX = toFloat.x / toFloat.w;
	float positionY = 0.1f + (toFloat.y / toFloat.w);
	EntityID damageUI = registry.CreateEntity(ENT_PERSIST_LOWEST);
	TransformComponent* transformComponent2 = registry.AddComponent<TransformComponent>(damageUI);
	UIComponent* uiElement2 = registry.AddComponent<UIComponent>(damageUI);
	if (damage < 1)
	{
		uiElement2->Setup("", "", "<1", DSFLOAT2(positionX, positionY), DSFLOAT2(512.0f, 512.0f), damageSize + 20.0f);
	}
	else
	{
		uiElement2->Setup("", "", std::to_string(damage).c_str(), DSFLOAT2(positionX, positionY), DSFLOAT2(512.0f, 512.0f), damageSize + 20.0f);
	}
	
	uiElement2->m_BaseImage.baseUI.SetVisibility(false);
	uiElement2->m_BaseImage.Release();//Release image to save space
	
	if (stateManager.player.index == entity.index)
	{
		uiElement2->m_BaseText.m_brush = LightSteelBlue;
	}
	else
	{
		uiElement2->m_BaseText.m_brush = Yellow;
	}
	FollowerComponent* follow = registry.AddComponent<FollowerComponent>(damageUI, entity);
	AddTimedEventComponentStartContinuousEnd(damageUI, 0.0f, nullptr, UIFollowTransform, time, DestroyEntity, (int)(damage == 0) , 2);
}