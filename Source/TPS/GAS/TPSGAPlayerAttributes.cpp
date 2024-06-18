//© 2024 Andrei Vinnik

#include "TPSGAPlayerAttributes.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"


void UTPSGAPlayerAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UTPSGAPlayerAttributes, Health, Params);
	DOREPLIFETIME_CONDITION(UTPSGAPlayerAttributes, MaxHealth, COND_InitialOnly);
	DOREPLIFETIME_WITH_PARAMS_FAST(UTPSGAPlayerAttributes, Stamina, Params);
	DOREPLIFETIME_CONDITION(UTPSGAPlayerAttributes, MaxStamina, COND_InitialOnly);
}

void UTPSGAPlayerAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		
		if (GetHealth() == 0)
		{
			OnDead.Broadcast();
		}

		MARK_PROPERTY_DIRTY_FROM_NAME(UTPSGAPlayerAttributes, Health, this);
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		MARK_PROPERTY_DIRTY_FROM_NAME(UTPSGAPlayerAttributes, Stamina, this);

		if (GetStamina() == 0)
		{
			OnStaminaOver.Broadcast();
		}
	}
}
