//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPSGABase.h"
#include "TPSGAAiming.generated.h"

UCLASS()
class TPS_API UTPSGAAiming : public UTPSGABase
{
	GENERATED_BODY()
public:
	UTPSGAAiming();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
