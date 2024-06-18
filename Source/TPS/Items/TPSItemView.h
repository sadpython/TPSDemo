//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSItemView.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UTPSItem;

UCLASS(Blueprintable, BlueprintType)
class TPS_API ATPSItemView : public AActor
{
	GENERATED_BODY()
	
public:
	ATPSItemView();

	virtual void BeginPlay() override;

	void OnPickup();
	void OnDrop();
	
	UTPSItem* GetItem() const { return Item; }

protected:
	UFUNCTION()
	void OnItemSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSItem> ItemClass;

	UPROPERTY(ReplicatedUsing=OnItemSet)
	UTPSItem* Item;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SKMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* PickupCollision;
};
