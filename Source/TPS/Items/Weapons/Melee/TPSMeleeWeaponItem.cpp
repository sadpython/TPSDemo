//© 2024 Andrei Vinnik

#include "TPSMeleeWeaponItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TPSMeleeWeaponItemSettings.h"
#include "TPS/Core/TPSCharacter.h"
#include "TPS/Items/Weapons/TPSDamageEffect.h"


void UTPSMeleeWeaponItem::ApplyDamage(AActor* InActor)
{
	if (GetPlayerCharacter()->HasAuthority())
	{
		if (auto OwnerPlayer = GetPlayerCharacter())
		{
			if (InActor)
			{
				if (InActor->Implements<UAbilitySystemInterface>())
				{
					auto AbilitySystem = Cast<IAbilitySystemInterface>(InActor)->GetAbilitySystemComponent();
					FGameplayEffectContextHandle EffectContext = OwnerPlayer->GetAbilitySystemComponent()->MakeEffectContext();
					EffectContext.AddSourceObject(this);

					FGameplayEffectSpecHandle NewHandle = OwnerPlayer->GetAbilitySystemComponent()->MakeOutgoingSpec(GetDamageEffect(), 0, EffectContext);
					auto Damage = GetItemSettings<UTPSMeleeWeaponItemSettings>()->GetDamage();
					NewHandle.Data->SetSetByCallerMagnitude("PhysDamage", Damage);
				
					if (NewHandle.IsValid())
					{
						FActiveGameplayEffectHandle ActiveGEHandle = OwnerPlayer->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);
					}
				}
			}
		}
	}
}
