#include "ComponentHelper.h"
#include "RelicFunctions.h"
#include "Relics/RelicFuncInputTypes.h"
#include "UIRenderer.h"

float StatComponent::GetHealth() const
{
	return this->currentHealth;
}

float StatComponent::GetMaxHealth() const
{
	return this->maximumHealth;
}

float StatComponent::GetHealthFraction() const
{
	return this->currentHealth / this->maximumHealth;
}

float StatComponent::UpdateHealth(const float delta)
{
	// If damage is being delt, apply the damage
	if (delta < 0)
		this->currentHealth += delta;
	// If healing is being applied, increase current health and then cap it at maximum health
	else if (0 < delta && this->currentHealth < this->maximumHealth)
	{
		this->currentHealth += delta;
		this->currentHealth = this->currentHealth < this->maximumHealth ? this->currentHealth : this->maximumHealth;
	}
	// Return the new current health
	return this->currentHealth;
}

float StatComponent::UpdateMaxHealth(const float delta)
{
	// Update maximum health
	this->maximumHealth += delta;

	// If the change is positive, heal
	if (0 < delta)
		UpdateHealth(delta);

	// Else if the change would make the entities current health greater than its maximum, subtract the difference
	else if (this->maximumHealth < this->currentHealth)
		UpdateHealth(this->maximumHealth - this->currentHealth);

	// Return the new max health
	return this->maximumHealth;
}

int PlayerComponent::UpdateSouls(const int delta)
{
	// Update the UI after the player gets a soul
	RedrawUI();

	auto onSoulUpdateFunctions = Relics::GetFunctionsOfType(Relics::FUNC_ON_SOUL_UPDATE);
	RelicInput::OnSoulUpdate input =
	{
		/*Soul Delta*/	delta
	};

	for (size_t i = 0; i < onSoulUpdateFunctions.size(); ++i)
	{
		onSoulUpdateFunctions[i](&input);
	}

	this->souls += delta;
	return this->souls;
}

int PlayerComponent::GetSouls() const
{
	return this->souls;
}