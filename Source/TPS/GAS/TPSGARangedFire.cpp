//© 2024 Andrei Vinnik

#include "TPSGARangedFire.h"

#include "TPS/Core/TPSCharacter.h"
#include "TPS/Items/Weapons/TPSWeaponItem.h"
#include "TPS/Items/Weapons/Ranged/TPSRangedWeaponItem.h"
#include "TPS/Items/Weapons/Ranged/TPSRangedWeaponItemSettings.h"

UTPSGARangedFire::UTPSGARangedFire()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UTPSGARangedFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = GetOwner();

	if (!Player->GetEquippedWeaponItem())
	{
		K2_EndAbility();
		return;
	}
	
	WeaponItem = Cast<UTPSRangedWeaponItem>(GetOwner()->GetEquippedWeaponItem());
	
	WeaponSettings = WeaponItem->GetItemSettings<UTPSRangedWeaponItemSettings>();

	if (!WeaponSettings)
	{
		K2_EndAbility();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [this]()
	{
		if (WeaponItem->HasAmmo())
		{
			WeaponItem->Fire();
		}
		else
		{
			K2_EndAbility();
		}
	}, WeaponSettings->GetFireRate(), true);
}

void UTPSGARangedFire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
}
