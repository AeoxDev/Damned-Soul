#pragma once
#include "EntityID.h"
#include "RelicFuncInputTypes.h"

RelicInput::OnDamageCalculation GetModifiers(EntityID attacker, EntityID defender);

RelicInput::OnDamageCalculation RetaliationCombination(const RelicInput::OnDamageCalculation& original, const RelicInput::OnDamageCalculation& roleReversed, const float fraction);