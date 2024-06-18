//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/TPSItem.h"
#include "TPS/Items/TPSItemSettings.h"
#include "TPS/Items/Weapons/TPSWeaponItem.h"
#include "TPSMeleeWeaponItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSMeleeWeaponItem : public UTPSWeaponItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(AActor* InActor);
};
