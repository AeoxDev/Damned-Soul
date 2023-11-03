#include "Components.h"
#include "Relics/RelicFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "UIRenderer.h"
#include "States\StateManager.h"

void StatComponent::MarkAsModified()
{
	m_modifiedStats = true;
}

bool StatComponent::IsModified() const
{
	return m_modifiedStats;
}

void StatComponent::ZeroBonusStats()
{
	m_bonusHealth = 0;
	m_damageReduction = 1.f; // Since this is a multiplier, setting it to 1.0 is equivalent to setting the bonus to 0
	m_bonusMoveSpeed = 0;
	m_bonusDamage = 0;
	m_bonusAttackSpeed = 0;
	m_bonusKnockback = 0;
}

float StatComponent::GetHealth() const
{
	return this->m_currentHealth;
}

float StatComponent::GetMaxHealth() const
{
	return m_baseHealth + m_bonusHealth;
}

float StatComponent::GetHealthFraction() const
{
	return this->m_currentHealth / GetMaxHealth();
}

float StatComponent::UpdateHealth(const float delta, const bool hitByEnemy)
{
	// If damage is being delt, apply the damage
	if (delta < 0)
	{
		// Damage is affected by damage reduction
		m_currentHealth += delta;
		if (hitByEnemy)
		{
			//Play hurt sound
			StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);
			if (stats == this)
			{
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player);
				if (sfx)
				{
					if (this->GetHealth() <= 0)
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
	else if (0 < delta && m_currentHealth < GetMaxHealth())
	{
		m_currentHealth += delta;
		m_currentHealth = m_currentHealth < GetMaxHealth() ? m_currentHealth : GetMaxHealth();
	}
	// Else, do nothing, only return
	else
	{
		return m_currentHealth;
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
	return m_currentHealth;
}

void StatComponent::UpdateBonusHealth(const float delta)
{
	m_bonusHealth += delta;
}

float StatComponent::GetSpeed() const
{
	float speed = (m_baseMoveSpeed + m_bonusMoveSpeed) * m_speedMult;
	return speed * (0.f < speed); // Branchlessly put a lower limit of zero on speed
}

void StatComponent::UpdateBonusSpeed(const float delta)
{
	m_bonusMoveSpeed += delta;
}

void StatComponent::SetSpeedMult(const float mult)
{
	m_speedMult = mult;
}

float StatComponent::GetDamage() const
{
	return m_baseDamage + m_bonusDamage;
}

void StatComponent::UpdateBonusDamage(const float delta)
{
	m_bonusDamage += delta;
}

float StatComponent::GetAttackSpeed() const
{
	return m_baseAttackSpeed + m_bonusAttackSpeed;
}

void StatComponent::UpdateBonusAttackSpeed(const float delta)
{
	m_bonusAttackSpeed += delta;
}

float StatComponent::GetKnockback() const
{
	return (m_baseKnockback + m_bonusKnockback) * m_knockbackMultiplier;
}

void StatComponent::UpdateBonusKnockback(const float delta)
{
	m_bonusKnockback += delta;
}

void StatComponent::SetKnockbackMultiplier(const float mult)
{
	m_knockbackMultiplier = mult;
}

//float StatComponent::UpdateMaxHealth(const float delta)
//{
//	// Update maximum health
//	this->m_baseHealth += delta;
//
//	// If the change is positive, heal
//	if (0 < delta)
//		UpdateHealth(delta);
//
//	// Else if the change would make the entities current health greater than its maximum, subtract the difference
//	else if (this->m_baseHealth < this->m_currentHealth)
//		UpdateHealth(this->m_baseHealth - this->m_currentHealth);
//
//	// Return the new max health
//	return this->m_baseHealth;
//}

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