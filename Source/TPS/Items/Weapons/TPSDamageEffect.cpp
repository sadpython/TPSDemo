//© 2024 Andrei Vinnik

#include "TPSDamageEffect.h"

#include "TPS/GAS/TPSGAPlayerAttributes.h"

struct TargetHealthCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)

	TargetHealthCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTPSGAPlayerAttributes, Health, Target, true)
	}
};

static TargetHealthCapture& GetTargetHealthCapture()
{
	static TargetHealthCapture TargetHealthCaptureVar;
	return TargetHealthCaptureVar;
}

UTPSGameplayEffectDamageCalc::UTPSGameplayEffectDamageCalc()
{
	RelevantAttributesToCapture.Add(GetTargetHealthCapture().HealthDef);
}

void UTPSGameplayEffectDamageCalc::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	float TargetHealthMagnitude = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetTargetHealthCapture().HealthDef, FAggregatorEvaluateParameters(), TargetHealthMagnitude);

	float Damage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude("PhysDamage");
	
	float OutTargetHealth = TargetHealthMagnitude - Damage;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetTargetHealthCapture().HealthProperty, EGameplayModOp::Override, OutTargetHealth));
}
