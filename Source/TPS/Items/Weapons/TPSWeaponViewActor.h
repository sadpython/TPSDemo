//© 2024 Andrei Vinnik

#pragma once
#include "TPS/Items/TPSItemView.h"
#include "TPSWeaponViewActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ATPSWeaponViewActor : public ATPSItemView
{
public:
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
