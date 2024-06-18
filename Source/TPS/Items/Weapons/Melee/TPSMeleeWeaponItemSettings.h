//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/Weapons/TPSWeaponItemSettings.h"
#include "TPSMeleeWeaponItemSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSMeleeWeaponItemSettings : public UTPSWeaponItemSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UAnimMontage* GetAnimMontage() const;
protected:

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> AttackAnimMontage;
};
