#include "Components.h"
#include "Relics/RelicFunctions.h"
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

float StatComponent::UpdateHealth(const float delta, const bool hitByEnemy)
{
	// If damage is being delt, apply the damage
	if (delta < 0)
	{
		this->currentHealth += delta;
		if (hitByEnemy)
		{
			//Play hurt sound
			for (auto entity : View<PlayerComponent>(registry))
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
				if (sfx)
				{
					if (this->currentHealth <= 0)
					{
						sfx->Play(Player_Death, Channel_Base);
					}
					else
					{
						sfx->Play(Player_Hurt, Channel_Base);
					}
				}
			}
		}
	}
	// If healing is being applied, increase current health and then cap it at maximum health
	else if (0 < delta && this->currentHealth < this->maximumHealth)
	{
		this->currentHealth += delta;
		this->currentHealth = this->currentHealth < this->maximumHealth ? this->currentHealth : this->maximumHealth;
	}
	// Else, do nothing, only return
	else
	{
		return this->currentHealth;
	}

	// Some sort of health mod happened, do the health mod relics
	auto hpUpdate = Relics::GetFunctionsOfType(Relics::FUNC_ON_HEALTH_MODIFIED);
	RelicInput::OnHealthUpdate hpUpdateData =
	{
		/*Delta Health*/		delta,
		/*Adress Of StatComp*/	this
	};
	for (uint32_t i = 0; i < hpUpdate.size(); ++i)
	{
		hpUpdate[i](&hpUpdateData);
	}
	RedrawUI();
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

	for (unsigned i = 0; i < onSoulUpdateFunctions.size(); ++i)
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