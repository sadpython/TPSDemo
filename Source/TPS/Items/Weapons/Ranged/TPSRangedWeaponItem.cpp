//© 2024 Andrei Vinnik

#include "TPSRangedWeaponItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "TPSRangedWeaponItemSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "TPS/Core/TPSCharacter.h"
#include "TPS/Items/Weapons/TPSDamageEffect.h"

void UTPSRangedWeaponItem::InitItem(UTPSItemSettings* InSettings)
{
	Super::InitItem(InSettings);
	auto WeaponSettings = Cast<UTPSRangedWeaponItemSettings>(ItemSettings);
	Ammo = WeaponSettings->GetInitialAmmo();
	AttackDistance = WeaponSettings->GetAttackDistance();
}

void UTPSRangedWeaponItem::Reload()
{
	auto WeaponSettings = Cast<UTPSRangedWeaponItemSettings>(ItemSettings);
	Ammo = WeaponSettings->GetInitialAmmo();
}

bool UTPSRangedWeaponItem::HasAmmo()
{
	return Ammo > 0;
}

void UTPSRangedWeaponItem::Fire()
{
	if (GetPlayerCharacter()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire"));
		if (auto OwnerPlayer = GetPlayerCharacter())
		{
			auto PC = OwnerPlayer->GetController();
			FVector startTrace;
			FRotator ownerRot;
			PC->GetPlayerViewPoint(startTrace, ownerRot);

			startTrace += ownerRot.GetNormalized().Vector() * OwnerPlayer->GetCameraBoom()->TargetArmLength;
			FCollisionQueryParams TraceParams;
		
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult, startTrace, startTrace + ownerRot.GetNormalized().Vector() * AttackDistance, ECollisionChannel::ECC_Camera, TraceParams, FCollisionResponseParams::DefaultResponseParam);
			DrawDebugLine(GetWorld(), startTrace, startTrace + ownerRot.GetNormalized().Vector() * AttackDistance, FColor::Yellow, true, -1, 0, 2.0f);

			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				if (HitActor->Implements<UAbilitySystemInterface>())
				{
					auto AbilitySystem = Cast<IAbilitySystemInterface>(HitActor)->GetAbilitySystemComponent();
					FGameplayEffectContextHandle EffectContext = OwnerPlayer->GetAbilitySystemComponent()->MakeEffectContext();
					EffectContext.AddSourceObject(this);

					FGameplayEffectSpecHandle NewHandle = OwnerPlayer->GetAbilitySystemComponent()->MakeOutgoingSpec(GetDamageEffect(), 0, EffectContext);
					auto Damage = GetItemSettings<UTPSRangedWeaponItemSettings>()->GetDamage();
					NewHandle.Data->SetSetByCallerMagnitude("PhysDamage", Damage);
				
					if (NewHandle.IsValid())
					{
						FActiveGameplayEffectHandle ActiveGEHandle = OwnerPlayer->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);
					}
				}
			}
			DecreaseAmmo();
		}
	}
}

void UTPSRangedWeaponItem::DecreaseAmmo()
{
	--Ammo;
	MARK_PROPERTY_DIRTY_FROM_NAME(UTPSRangedWeaponItem, Ammo, this);
	OnAmmoChanged();
}

void UTPSRangedWeaponItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UTPSRangedWeaponItem, Ammo, Params);
}

void UTPSRangedWeaponItem::OnAmmoChanged()
{
	OnAmmoChangedDelegate.Broadcast();
	OnAmmoChangedDelegateNative.Broadcast();
}
