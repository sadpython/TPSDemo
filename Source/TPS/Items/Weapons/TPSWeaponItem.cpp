//© 2024 Andrei Vinnik

#include "TPSWeaponItem.h"

#include "TPSWeaponItemSettings.h"

TSubclassOf<UTPSDamageEffect> UTPSWeaponItem::GetDamageEffect() const
{
	return GetItemSettings<UTPSWeaponItemSettings>()->GetDamageEffect();
}

TSubclassOf<UTPSGAAiming> UTPSWeaponItem::GetAimAbility() const
{
	return GetItemSettings<UTPSWeaponItemSettings>()->GetAimAbility();
}

TSubclassOf<UTPSGABase> UTPSWeaponItem::GetAttackAbility() const
{
	return GetItemSettings<UTPSWeaponItemSettings>()->GetAttackAbility();
}

bool UTPSWeaponItem::CouldCancelByInput() const
{
	return GetItemSettings<UTPSWeaponItemSettings>()->CouldCancelAttackByInput();
}
