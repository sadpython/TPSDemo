//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "UObject/Object.h"
#include "TPSDamageEffect.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()
};

UCLASS(Blueprintable, BlueprintType)
class TPS_API UTPSGameplayEffectDamageCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UTPSGameplayEffectDamageCalc();
protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};