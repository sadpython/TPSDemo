//© 2024 Andrei Vinnik

#include "TPSWeaponItemSettings.h"

TSubclassOf<UTPSGABase> UTPSWeaponItemSettings::GetAttackAbility() const
{
	return AttackAbility.LoadSynchronous();
}

TSubclassOf<UTPSGAAiming> UTPSWeaponItemSettings::GetAimAbility() const
{
	return AimAbility.LoadSynchronous();
}

TSubclassOf<UTPSDamageEffect> UTPSWeaponItemSettings::GetDamageEffect() const
{
	return DamageEffectClass.LoadSynchronous();
}
