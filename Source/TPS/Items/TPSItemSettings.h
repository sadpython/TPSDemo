//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TPSItemSettings.generated.h"

class UTPSItem;

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSItemSettings : public UObject
{
	GENERATED_BODY()

public:
	TSubclassOf<UTPSItem> GetItemClass() const;
	FText GetItemName() const;
	UTexture2D* GetIcon() const;
	USkeletalMesh* GetMesh() const;
protected:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSItem> ItemClass;

	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> Mesh;
};
