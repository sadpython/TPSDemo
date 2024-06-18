//© 2024 Andrei Vinnik

#include "TPSGAAiming.h"

#include "Camera/CameraComponent.h"
#include "TPS/Core/TPSCharacter.h"

UTPSGAAiming::UTPSGAAiming()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UTPSGAAiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Character = Cast<ATPSCharacter>(GetAvatarActorFromActorInfo());
	if (!HasAuthority(&ActivationInfo))
	{
		
		Character->GetFollowCamera()->SetFieldOfView(50);
	}
	Character->bUseControllerRotationYaw = true;
}

void UTPSGAAiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	auto Character = Cast<ATPSCharacter>(GetAvatarActorFromActorInfo());
	if (!HasAuthority(&ActivationInfo))
	{
		
		Character->GetFollowCamera()->SetFieldOfView(90);
	}
	Character->bUseControllerRotationYaw = false;
}
