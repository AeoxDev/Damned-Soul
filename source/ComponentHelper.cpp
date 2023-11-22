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
	m_bonusDashValue = 0;
	m_bonusDamage = 0;
	m_bonusAttackSpeed = 0;
	m_bonusKnockback = 0;
}

int64_t StatComponent::GetHealth() const
{
	return ceil(this->m_currentHealth);
}

int64_t StatComponent::GetMaxHealth() const
{
	return ceil(m_baseHealth + m_bonusHealth);
}

float StatComponent::GetHealthFraction() const
{
	return this->m_currentHealth / GetMaxHealth();
}

float StatComponent::CapHealth()
{
	float maxHp = GetMaxHealth();
	bool contained = m_currentHealth < maxHp;

	// Branchless limit
	m_currentHealth = (m_currentHealth * contained) + (maxHp * !contained);
	// Return current health
	return m_currentHealth;
}

float StatComponent::StealthilyModifyHealth(const float delta)
{
	//// If damage is being delt, apply the damage
	//if (delta < 0)
	//{
	//	// Damage is affected by damage reduction
	//	m_currentHealth += delta;
	//}
	//// If healing is being applied, increase current health and then cap it at maximum health
	//else if (0 < delta && m_currentHealth < GetMaxHealth())
	//{
	//	m_currentHealth += delta;
	//	m_currentHealth = m_currentHealth < GetMaxHealth() ? m_currentHealth : GetMaxHealth();
	//}
	//// Else, do nothing, only return
	//else
	//{
	//	return m_currentHealth;
	//}

	//// Some sort of health mod happened, do the health mod relics
	//auto hpUpdate = Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_TAKEN);
	//RelicInput::OnHealthUpdate hpUpdateData =
	//{
	//	/*Adress Of StatComp*/	this,
	//	/*Delta Health*/		delta
	//};
	//for (uint32_t i = 0; i < hpUpdate.size(); ++i)
	//{
	//	hpUpdate[i](&hpUpdateData);
	//}
	//RedrawUI();
	//// Return the new current health
	//return m_currentHealth;

	m_currentHealth += delta;
	CapHealth();
	RedrawUI();

	// Return current health
	return m_currentHealth;
}

float StatComponent::ApplyDamage(const float damage, const bool hitByEnemy)
{
	if (0 < damage)
	{
		// Apply damage as a negative delta
		StealthilyModifyHealth(-damage);

		// Apply on damage taken effects
		RelicInput::OnHealthUpdate hpUpdateData =
		{
			/*Adress Of StatComp*/	this,
			/*Delta Health*/		damage
		};
		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_TAKEN))
			func(&hpUpdateData);
	}

	// Return current health
	// Can potentially return negative
	return GetHealth();
}

float StatComponent::ApplyHealing(const float healing, const bool hitByEnemy)
{
	if (0 < healing && m_currentHealth < GetMaxHealth())
	{
		// Apply healing as a positive delta
		StealthilyModifyHealth(+healing);

		// Apply on healing applied effects
		RelicInput::OnHealthUpdate hpUpdateData =
		{
			/*Adress Of StatComp*/	this,
			/*Delta Health*/		healing
		};
		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_HEALING_TAKEN))
			func(&hpUpdateData);
	}

	// Return current health
	// Can potentially return greater than max, but StatCalcSystem does limit current health to max health when calculating stats later
	return GetHealth();
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

float StatComponent::GetDashDistance() const
{
	return m_baseDashValue + m_bonusDashValue;
}

void StatComponent::UpdateBonusDashDistance(const float delta)
{
	m_bonusDashValue += delta;
}

float StatComponent::GetBaseDamage() const
{
	return m_baseDamage;
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

	if (delta > 0)
		this->totalSouls += delta;

	this->souls += delta;
	return this->souls;
}

int PlayerComponent::GetSouls() const
{
	return this->souls;
}

int PlayerComponent::GetTotalSouls() const
{
	return this->totalSouls;
}
