//© 2024 Andrei Vinnik

#include "TPSGAMeleeFire.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS/Core/TPSCharacter.h"
#include "TPS/Items/Weapons/Melee/TPSMeleeWeaponItem.h"
#include "TPS/Items/Weapons/Melee/TPSMeleeWeaponItemSettings.h"
#include "TPS/Items/Weapons/Ranged/TPSRangedWeaponItem.h"

UTPSGAMeleeFire::UTPSGAMeleeFire()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UTPSGAMeleeFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	auto Player = GetOwner();
	if (!Player->GetEquippedWeaponItem())
	{
		K2_EndAbility();
		return;
	}
	
	WeaponItem = Cast<UTPSMeleeWeaponItem>(Player->GetEquippedWeaponItem());
	
	WeaponSettings = WeaponItem->GetItemSettings<UTPSMeleeWeaponItemSettings>();
	
	if (!WeaponSettings)
	{
		K2_EndAbility();
		return;
	}

	Player->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UTPSGAMeleeFire::OnMontageNotify);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTPSGAMeleeFire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	auto Player = GetOwner();
	Player->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveAll(this);
}

void UTPSGAMeleeFire::OnMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnWeaponMontage(NotifyName);
}
