#include "Relics\Utility\RelicFuncInputTypes.h"

float RelicInput::OnDamageCalculation::CollapseDamage()
{
    // First add flat damage increase
    float retVal = (damage + flatAdd);
    // Multiply damage
    retVal *= incMult * decMult;
    // Apply flat damage decrease
    retVal -= flatSub;

    // Contain the damage in a range of 0 to current health
    retVal = (retVal < cap) ? retVal : cap;
    retVal = (0 < retVal) ? retVal : 0;
    
    // Return the damage
    return retVal;
}

float RelicInput::OnDamageCalculation::CollapseNoCap()
{
    // First add flat damage increase
    float retVal = (damage + flatAdd);
    // Multiply damage
    retVal *= incMult * decMult;
    // Apply flat damage decrease
    retVal -= flatSub;

    // Return the damage
    return retVal;
}

int32_t RelicInput::OnPriceCalculation::GetCostOf(const int32_t initial, const PRICE_TYPES type)
{
    float finalMod = everythingCostMult;
    if (type & RELIC)
        finalMod *= relicCostMult;
    if (type & HEAL)
        finalMod *= healCostMult;
    if (type & REROLL)
        finalMod *= rerollCostMult;
    if (type & UPGRADE)
        finalMod *= upgradeCost;

    int32_t retVal = std::max(0, int32_t(.5f + initial * finalMod));

    return retVal;
}
