//© 2024 Andrei Vinnik

#include "TPSItemView.h"

#include "TPSItem.h"
#include "TPSItemSettings.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

ATPSItemView::ATPSItemView()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SKMesh->SetupAttachment(RootComponent);
	SKMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	PickupCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	PickupCollision->SetupAttachment(RootComponent);
	bReplicateUsingRegisteredSubObjectList = true;
	bReplicates = true;
	SetReplicatingMovement(true);
}

void ATPSItemView::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (auto ItemCls = ItemClass.LoadSynchronous())
		{
			Item = NewObject<UTPSItem>(this, ItemCls);
			Item->InitItemWithDefault();
			Item->SetOwnerActor(this);
			AddReplicatedSubObject(Item);
			MARK_PROPERTY_DIRTY_FROM_NAME(ATPSItemView, Item, this);
			OnItemSet();
		}
	}
}

void ATPSItemView::OnPickup()
{
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	RemoveReplicatedSubObject(Item);
}

void ATPSItemView::OnDrop()
{
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	AddReplicatedSubObject(Item);
}

void ATPSItemView::OnItemSet()
{
	SKMesh->SetSkeletalMesh(Item->GetItemSettings()->GetMesh());
}

void ATPSItemView::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ATPSItemView, Item, Params);
}

