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
	//m_bonusDashValue = 0;
	m_bonusDamage = 0;
	m_bonusAttackSpeed = 0;
	m_bonusKnockback = 0;
}

int64_t StatComponent::GetHealth() const
{
	return (int64_t)ceil(GetMaxHealth() - this->m_damageTaken);
	//return (int64_t)ceil(this->m_currentHealth);
}

int64_t StatComponent::GetMaxHealth() const
{
	return (int64_t)ceil(m_baseHealth + m_bonusHealth);
}

int64_t StatComponent::GetBonusHealth() const
{
	return (int64_t)ceil(m_bonusHealth);
}

float StatComponent::GetHealthFraction() const
{
	float maxHp = (float)GetMaxHealth();
	return (maxHp - m_damageTaken) / maxHp;
	//return this->m_currentHealth / GetMaxHealth();
}

float StatComponent::GetHealthRecovered() const
{
	return m_HealthRecovered;
}

float StatComponent::GetDamageTaken() const
{
	return m_TotalDamageTaken;
}

void StatComponent::UpdateBaseHealth(const float delta)
{
	m_baseHealth += delta;
}

float StatComponent::CapHealth()
{
	float maxHp = (float)GetMaxHealth();
	bool contained = 0.f < m_damageTaken;

	// Branchless limit
	m_damageTaken = (m_damageTaken * contained);
	// Return current health
	return (float)GetHealth();

	//float maxHp = (float)GetMaxHealth();
	//bool contained = m_currentHealth < maxHp;

	//// Branchless limit
	//m_currentHealth = (m_currentHealth * contained) + (maxHp * !contained);
	//// Return current health
	//return m_currentHealth;
}

float StatComponent::StealthilyModifyHealth(const float delta)
{
	m_damageTaken -= delta;
	CapHealth();
	RedrawUI();

	// Return current health
	return (float)GetHealth();

	//m_currentHealth += delta;
	//CapHealth();
	//RedrawUI();

	//// Return current health
	//return m_currentHealth;
}

float StatComponent::ApplyDamage(const float damage, const bool hitByEnemy)
{
	if (0 < damage)
	{
		// Apply damage as a negative delta
		StealthilyModifyHealth(-damage);
		m_TotalDamageTaken += damage;

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
	return (float)GetHealth();
}

float StatComponent::ApplyHealing(const float healing, const bool hitByEnemy)
{
	if (0 < healing && 0 < m_damageTaken)
	{
		// Apply healing as a positive delta
		StealthilyModifyHealth(+healing);
		m_HealthRecovered += healing;

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
	return (float)GetHealth();

	//if (0 < healing && m_currentHealth < GetMaxHealth())
	//{
	//	// Apply healing as a positive delta
	//	StealthilyModifyHealth(+healing);
	//	m_HealthRecovered += healing;

	//	// Apply on healing applied effects
	//	RelicInput::OnHealthUpdate hpUpdateData =
	//	{
	//		/*Adress Of StatComp*/	this,
	//		/*Delta Health*/		healing
	//	};
	//	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_HEALING_TAKEN))
	//		func(&hpUpdateData);
	//}

	//// Return current health
	//// Can potentially return greater than max, but StatCalcSystem does limit current health to max health when calculating stats later
	//return (float)GetHealth();
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

float StatComponent::GetBaseSpeed() const
{
	return m_baseMoveSpeed;
}

float StatComponent::GetBonusSpeed() const
{
	return GetSpeed() - m_baseMoveSpeed;
}

void StatComponent::UpdateBonusSpeed(const float delta)
{
	m_bonusMoveSpeed += delta * m_baseMoveSpeed;
}

void StatComponent::SetSpeedMult(const float mult)
{
	m_speedMult = mult;
}

//float StatComponent::GetDashDistance() const
//{
//	return m_baseDashValue + m_bonusDashValue;
//}
//
//void StatComponent::UpdateBonusDashDistance(const float delta)
//{
//	m_bonusDashValue += delta;
//}

float StatComponent::GetBaseDamage() const
{
	return m_baseDamage;
}

void StatComponent::UpdateBaseDamage(const float delta)
{
	m_baseDamage += delta;
}

float StatComponent::GetDamage() const
{
	return m_baseDamage + m_bonusDamage;
}

void StatComponent::UpdateBonusDamage(const float delta)
{
	m_bonusDamage += delta;
}

float StatComponent::GetBonusDamage() const
{
	return m_bonusDamage;
}

void StatComponent::UpdateDamageDone(const float delta)
{
	m_TotalDamageDone += delta;
}

float StatComponent::GetDamageDone() const
{
	return m_TotalDamageDone;
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
	{
		this->totalSouls += delta;

		//Play sound effect occasionally when we get a new soul.
		int playerSoundEffect = rand() % 16;
		if (playerSoundEffect == 0)
		{
			for (auto entity : View<PlayerComponent>(registry))
			{
				bool isPlaying = false;
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
				AudioEngineComponent* audioJungle = nullptr;
				for (auto jungle : View<AudioEngineComponent>(registry))
				{
					audioJungle = registry.GetComponent<AudioEngineComponent>(jungle);
				}

				if (audioJungle != nullptr)
				{
					if (sfx != nullptr)
					{
						audioJungle->channels[sfx->channelIndex[Channel_Extra]]->isPlaying(&isPlaying);
						if (!isPlaying)
						{
							playerSoundEffect = rand() % 2;
							if (playerSoundEffect == 0)
							{
								sfx->Play(Player_BetterLuck, Channel_Extra);
							}
							else
							{
								sfx->Play(Player_AnotherSoul, Channel_Extra);
							}
						}
					}
				}
			}
		}
	}

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

void PlayerComponent::UpdateBonusChargeRate(const float delta)
{
	m_chargeRate += delta;
}

float PlayerComponent::GetChargeRate() const
{
	return m_chargeRate;
}

void PlayerComponent::UpdateBonusDashScaling(const float delta)
{
	m_bonusDashValue += delta;
}

void PlayerComponent::UpdateBonusDashes(const int delta)
{
	m_bonusDashes += delta;
}

bool PlayerComponent::ConsumeDash()
{
	if (m_remainingDashes == m_baseDashes + m_bonusDashes)
		m_dashCounter = m_dashCooldown;
	return 0 <= --m_remainingDashes;
}

void PlayerComponent::DashCooldown(const float dt)
{
	if (m_dashCounter < dt)
	{
		m_dashCounter = 0;
		m_remainingDashes = m_baseDashes + m_bonusDashes;
	}
	else if (0 < m_dashCounter)
		m_dashCounter -= dt;
}

float PlayerComponent::GetDashValue()
{
	return m_baseDashValue + m_bonusDashValue;
}

void PlayerComponent::ZeroBonusStats()
{
	m_chargeRate = 1.f;
	m_bonusDashes = 0;
	m_bonusDashValue = 0;
}