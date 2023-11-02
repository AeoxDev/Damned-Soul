#include "Relics/RelicFuncInputTypes.h"

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