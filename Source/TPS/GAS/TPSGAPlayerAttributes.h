//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TPSGAPlayerAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TPS_API UTPSGAPlayerAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FGameplayAttributeData	Health;
	ATTRIBUTE_ACCESSORS(UTPSGAPlayerAttributes, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FGameplayAttributeData	MaxHealth;
	ATTRIBUTE_ACCESSORS(UTPSGAPlayerAttributes, MaxHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FGameplayAttributeData	Stamina;
	ATTRIBUTE_ACCESSORS(UTPSGAPlayerAttributes, Stamina);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FGameplayAttributeData	MaxStamina;
	ATTRIBUTE_ACCESSORS(UTPSGAPlayerAttributes, MaxStamina);
	
	FSimpleMulticastDelegate OnDead;
	FSimpleMulticastDelegate OnStaminaOver;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};
