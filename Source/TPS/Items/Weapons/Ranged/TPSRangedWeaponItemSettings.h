//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/Weapons/TPSWeaponItemSettings.h"
#include "TPSRangedWeaponItemSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSRangedWeaponItemSettings : public UTPSWeaponItemSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 GetAttackDistance() const { return AttackDistance; }
	UFUNCTION(BlueprintPure)
	int32 GetInitialAmmo() const { return Ammo; }
	UFUNCTION(BlueprintPure)
	float GetFireRate() const { return FireRatePerSecond; }
protected:
	UPROPERTY(EditAnywhere)
	int32 AttackDistance = 400;

	UPROPERTY(EditAnywhere)
	float FireRatePerSecond = 1;

	UPROPERTY(EditAnywhere)
	int32 Ammo;
};
