//© 2024 Andrei Vinnik

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "TPSGABase.generated.h"

class ATPSCharacter;

UCLASS()
class TPS_API UTPSGABase : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	
	ATPSCharacter* GetOwner() const;
};
