//© 2024 Andrei Vinnik

#include "TPSWeaponViewActor.h"

#include "Components/SphereComponent.h"
#include "TPS/Core/TPSCharacter.h"

void ATPSWeaponViewActor::BeginPlay()
{
	Super::BeginPlay();
	PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ATPSWeaponViewActor::OnOverlap);
}

void ATPSWeaponViewActor::OnOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (auto Character = Cast<ATPSCharacter>(OtherActor))
		{
			Character->PickupWeapon(this);
		}
	}
}
