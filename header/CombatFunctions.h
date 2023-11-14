#pragma once
#include "EntityID.h"
#include "Components.h"

namespace Combat
{
	// Provide a complete hit interaction between an attacker and a defender, applying damage, relics, and damage flashes
	void HitInteraction(EntityID& attacker, StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats, bool isCharged = false);

	//Works like hit interaction, but happens specifically when we dash into an enemy rather than hitting them
	void DashHitInteraction(EntityID& attacker, StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats);

	// Damage an entity and apply a damage flash. No relics are applied.
	void HitFlat(EntityID& defender, StatComponent* defenderStats, const float damage);

	// Apply a damage flash according to an instance of (hypothetical) damage
	void DamageFlash(EntityID& defender, const float damage);
}
