//© 2024 Andrei Vinnik

#include "TPSMeleeWeaponItemSettings.h"

UAnimMontage* UTPSMeleeWeaponItemSettings::GetAnimMontage() const
{
	return AttackAnimMontage.LoadSynchronous();
}
