//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TPSItem.generated.h"

class ATPSCharacter;
class UTPSItemSettings;

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSItem : public UObject
{
	GENERATED_BODY()

	friend class ATPSItemView;
public:
	UFUNCTION()
	virtual void InitItem(UTPSItemSettings* InSettings);

	UFUNCTION()
	void InitItemWithDefault();

	template<class T = UTPSItemSettings>
	T* GetItemSettings() const { return Cast<T>(ItemSettings); }

	virtual ATPSCharacter* GetPlayerCharacter();
	virtual AActor* GetOwnerActor();
	virtual UWorld* GetWorld() const override;

	void SetOwnerActor(AActor* InActor);
protected:
	//~ Begin UObject
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPSItemSettings> DefaultSettings;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	UTPSItemSettings* ItemSettings;

	UPROPERTY(Replicated)
	AActor* OwnedBy;

private:
	UFUNCTION(BlueprintPure, DisplayName="GetItemSettings")
	UTPSItemSettings* BP_GetItemSettings() const { return ItemSettings; }
};
