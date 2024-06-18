//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "TPS/Items/TPSItem.h"
#include "TPS/Items/Weapons/TPSWeaponItem.h"
#include "TPSRangedWeaponItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChangedDelegate);

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSRangedWeaponItem : public UTPSWeaponItem
{
	GENERATED_BODY()

public:
	virtual void InitItem(UTPSItemSettings* InSettings) override;

	virtual void Reload();
	virtual bool HasAmmo();
	virtual void Fire();
	virtual void DecreaseAmmo();

	FSimpleMulticastDelegate OnAmmoChangedDelegateNative;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing=OnAmmoChanged, BlueprintReadOnly)
	int32 Ammo;

	int32 AttackDistance;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChangedDelegate OnAmmoChangedDelegate;
	
	UFUNCTION()
	void OnAmmoChanged();
};
