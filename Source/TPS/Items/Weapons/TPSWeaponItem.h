//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/TPSItem.h"
#include "TPSWeaponItem.generated.h"

class UTPSGABase;
class UTPSGAAiming;
class UTPSDamageEffect;

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSWeaponItem : public UTPSItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TSubclassOf<UTPSDamageEffect> GetDamageEffect() const;
	UFUNCTION(BlueprintPure)
	TSubclassOf<UTPSGAAiming> GetAimAbility() const;
	UFUNCTION(BlueprintPure)
	TSubclassOf<UTPSGABase> GetAttackAbility() const;
	UFUNCTION(BlueprintPure)
	bool CouldCancelByInput() const;
};
