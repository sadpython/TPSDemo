//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPSGABase.h"
#include "TPSGAMeleeFire.generated.h"

class UTPSMeleeWeaponItemSettings;
class UTPSMeleeWeaponItem;

UCLASS()
class TPS_API UTPSGAMeleeFire : public UTPSGABase
{
	GENERATED_BODY()

public:
	UTPSGAMeleeFire();
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION()
	void OnMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponMontage(FName NotifyName);
	
	UPROPERTY(BlueprintReadOnly)
	UTPSMeleeWeaponItem* WeaponItem;
	
	UPROPERTY(BlueprintReadOnly)
	UTPSMeleeWeaponItemSettings* WeaponSettings;
};
