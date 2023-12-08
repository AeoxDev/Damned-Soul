#pragma once
#include "EntityID.h"
#include "Components.h"

void DamageNumbers(EntityID& defender, float damage);
void DamageNumbersDOT(EntityID& defender, float damageOverTime);
void DamageNumbersDOTRemainder(EntityID& defender);

namespace Combat
{
	// Provide a complete hit interaction between an attacker and a defender, applying damage, relics, and damage flashes
	void HitInteraction(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats/*, bool isCharged = false*/);

	// Calculate damage (presumably of a DoT efect), but do NOT apply it
	float CalculateDamage(const DamageOverTime& dot, EntityID& defender, const uint64_t& source);

	// Calculate damage (applying relics and such), but do NOT apply it
	float CalculateDamage(const EntityID& attacker, const StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats, const uint64_t& source);

	//Works like hit interaction, but happens specifically when we dash into an enemy rather than hitting them
	void DashHitInteraction(EntityID& attacker, StatComponent* attackerStats, EntityID& defender, StatComponent* defenderStats);

	// Damage an entity and apply a damage flash. No relics are applied.
	void HitFlat(EntityID& defender, StatComponent* defenderStats, const float damage, float time = 1.0f, bool isPlayer = false);

	// Apply a damage flash according to an instance of (hypothetical) damage
	void DamageFlash(EntityID& defender, const float damage);
}
