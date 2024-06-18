//© 2024 Andrei Vinnik

#include "TPSGABase.h"

#include "TPS/Core/TPSCharacter.h"

ATPSCharacter* UTPSGABase::GetOwner() const
{
	return Cast<ATPSCharacter>(GetAvatarActorFromActorInfo());
}
