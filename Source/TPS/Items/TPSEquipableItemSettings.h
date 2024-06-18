//© 2024 Andrei Vinnik

#pragma once
#include "TPSItemSettings.h"
#include "TPSEquipableItemSettings.generated.h"

UCLASS()
class UTPSEquipableItemSettings : public UTPSItemSettings
{
	GENERATED_BODY()
public:

	FName GetEquipmentSocket() const { return EquipmentSocket; }
protected:
	UPROPERTY(EditAnywhere)
	FName EquipmentSocket;
};
