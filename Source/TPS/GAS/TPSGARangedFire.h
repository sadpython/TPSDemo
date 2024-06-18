//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPSGABase.h"
#include "TPSGARangedFire.generated.h"

class UTPSRangedWeaponItem;
class UTPSRangedWeaponItemSettings;
class UTPSWeaponItem;

UCLASS()
class TPS_API UTPSGARangedFire : public UTPSGABase
{
	GENERATED_BODY()
public:
	UTPSGARangedFire();
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY()
	UTPSRangedWeaponItem* WeaponItem;
	UPROPERTY()
	UTPSRangedWeaponItemSettings* WeaponSettings;
	UPROPERTY()
	ATPSCharacter* Player;
	
	FTimerHandle AttackTimer;
};
