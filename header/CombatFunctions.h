#pragma once
#include "EntityID.h"
#include "Components.h"

namespace Combat
{
	// Provide a complete hit interaction between an attacker and a defender, applying damage, relics, and damage flashes
	void HitInteraction(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats);

	// Calculate damage (presumably of a DoT efect), but do NOT apply it
	float CalculateDamage(const DamageOverTimeComponent* dot, EntityID& defender, const uint64_t& source);

	// Calculate damage (applying relics and such), but do NOT apply it
	float CalculateDamage(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats, const uint64_t& source);

	// Damage an entity and apply a damage flash. No relics are applied.
	void HitFlat(EntityID& defender, StatComponent* defenderStats, const float damage);

	// Apply a damage flash according to an instance of (hypothetical) damage
	void DamageFlash(EntityID& defender, const float damage);
}
