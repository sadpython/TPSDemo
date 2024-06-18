//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/TPSEquipableItemSettings.h"
#include "TPSWeaponItemSettings.generated.h"

class UTPSDamageEffect;
class UTPSGAAiming;
class UTPSGABase;

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSWeaponItemSettings : public UTPSEquipableItemSettings
{
	GENERATED_BODY()

public:
	int32 GetDamage() const { return Damage; }
	TSubclassOf<UTPSGABase> GetAttackAbility() const;
	TSubclassOf<UTPSGAAiming> GetAimAbility() const;
	TSubclassOf<UTPSDamageEffect> GetDamageEffect() const;
	bool CouldCancelAttackByInput() const { return bCancelAttackOnInputRelease; }
protected:
	UPROPERTY(EditAnywhere)
	int32 Damage = 2;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSGABase> AttackAbility;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSDamageEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSGAAiming> AimAbility;
	
	UPROPERTY(EditAnywhere)
	bool bCancelAttackOnInputRelease = false;
};
