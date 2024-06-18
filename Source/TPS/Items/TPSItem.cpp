//© 2024 Andrei Vinnik

#include "TPSItem.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "TPS/Core/TPSCharacter.h"

void UTPSItem::InitItem(UTPSItemSettings* InSettings)
{
	ItemSettings = InSettings;
	MARK_PROPERTY_DIRTY_FROM_NAME(UTPSItem, ItemSettings, this);
}

void UTPSItem::InitItemWithDefault()
{
	InitItem(DefaultSettings.GetDefaultObject());
}

int32 UTPSItem::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !IsSupportedForNetworking())
	{
		return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
	}
	if (AActor* OwnerActor = Cast<AActor>(GetOuter()))
	{
		return OwnerActor->GetFunctionCallspace(Function, Stack);
	}
	return Super::GetFunctionCallspace(Function, Stack);
}

bool UTPSItem::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));

	bool bProcessed = false;
	AActor* OwnerActor = Cast<AActor>(GetOuter());
	FWorldContext* const Context = OwnerActor ? GEngine->GetWorldContextFromWorld(OwnerActor->GetWorld()) : nullptr;
	if (Context)
	{
		for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
		{
			if (Driver.NetDriver && Driver.NetDriver->ShouldReplicateFunction(OwnerActor, Function))
			{
				Driver.NetDriver->ProcessRemoteFunction(OwnerActor, Function, Parameters, OutParms, Stack, this);
				bProcessed = true;
			}
		}
	}

	return bProcessed;
}

void UTPSItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ItemSettings, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OwnedBy, Params);
}

ATPSCharacter* UTPSItem::GetPlayerCharacter()
{
	return Cast<ATPSCharacter>(OwnedBy);
}

AActor* UTPSItem::GetOwnerActor()
{
	return OwnedBy;
}

UWorld* UTPSItem::GetWorld() const
{
	return Cast<AActor>(OwnedBy)->GetWorld();
}

void UTPSItem::SetOwnerActor(AActor* InActor)
{
	OwnedBy = InActor;
	MARK_PROPERTY_DIRTY_FROM_NAME(UTPSItem, OwnedBy, this);
}
