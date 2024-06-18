//© 2024 Andrei Vinnik

#include "TPSCharacter.h"

#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TPS/GAS/TPSGAAiming.h"
#include "TPS/GAS/TPSGABase.h"
#include "TPS/GAS/TPSGAPlayerAttributes.h"
#include "TPS/GAS/Sprint/TPSGASprint.h"
#include "TPS/Items/TPSItemView.h"
#include "TPS/Items/Weapons/TPSWeaponItem.h"
#include "TPS/Items/Weapons/TPSWeaponItemSettings.h"
#include "TPS/Items/Weapons/Melee/TPSMeleeWeaponItem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATPSCharacter

ATPSCharacter::ATPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");

	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
}

void ATPSCharacter::PickupWeapon(ATPSItemView* InItemView)
{
	if (bIsPickingUp)
	{
		return;
	}
	bIsPickingUp = true;
	if (EquippedWeaponItem)
	{
		DropWeapon();
	}

	InItemView->OnPickup();
	EquippedWeaponItem = Cast<UTPSWeaponItem>(InItemView->GetItem());
	EquippedWeaponItem->SetOwnerActor(this);
	AddReplicatedSubObject(EquippedWeaponItem);
	MARK_PROPERTY_DIRTY_FROM_NAME(ATPSCharacter, EquippedWeaponItem, this);

	if (EquippedWeaponItem->GetAimAbility())
	{
		auto Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(EquippedWeaponItem->GetAimAbility());
		AbilitySystemComponent->GiveAbility(Spec);
	}

	if (EquippedWeaponItem->GetAttackAbility())
	{
		auto Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(EquippedWeaponItem->GetAttackAbility());
		AbilitySystemComponent->GiveAbility(Spec);
	}
	EquippedWeaponActor = InItemView;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATPSCharacter, EquippedWeaponActor, this);

	auto Settings = EquippedWeaponItem->GetItemSettings<UTPSWeaponItemSettings>();
	EquippedWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Settings->GetEquipmentSocket());
	
	bIsPickingUp = false;
}

void ATPSCharacter::DropWeapon()
{
	EquippedWeaponItem->SetOwnerActor(EquippedWeaponActor);
	
	EquippedWeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	EquippedWeaponActor->SetActorLocation(EquippedWeaponActor->GetActorLocation() + GetActorForwardVector() * 200.f);
	EquippedWeaponActor->OnDrop();
	
	EquippedWeaponActor = nullptr;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATPSCharacter, EquippedWeaponActor, this);
	
	RemoveReplicatedSubObject(EquippedWeaponItem);
	EquippedWeaponItem = nullptr;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATPSCharacter, EquippedWeaponItem, this);
	
	AbilitySystemComponent->CancelAbilityHandle(AttackHandle);
	AttackHandle = FGameplayAbilitySpecHandle();
	
	AbilitySystemComponent->CancelAbilityHandle(AimHandle);
	AimHandle = FGameplayAbilitySpecHandle();
}

void ATPSCharacter::LeftMouseAction(const FInputActionValue& Value)
{
	if (!EquippedWeaponItem)
	{
		return;
	}
	
	if (Value.Get<bool>())
	{
		AbilitySystemComponent->TryActivateAbilityByClass(EquippedWeaponItem->GetAttackAbility());
	}
	else
	{
		if (EquippedWeaponItem->CouldCancelByInput())
		{
			AbilitySystemComponent->CancelAbility(AbilitySystemComponent->FindAbilitySpecFromClass(EquippedWeaponItem->GetAttackAbility())->Ability);
		}
	}
}

void ATPSCharacter::RightMouseAction(const FInputActionValue& Value)
{
	if (!EquippedWeaponItem)
	{
		return;
	}
	if (EquippedWeaponItem->GetAimAbility())
	{
		if (Value.Get<bool>())
		{
			AbilitySystemComponent->TryActivateAbilityByClass(EquippedWeaponItem->GetAimAbility());
		}
		else
		{
			AbilitySystemComponent->CancelAbility(AbilitySystemComponent->FindAbilitySpecFromClass(EquippedWeaponItem->GetAimAbility())->Ability);
		}
	}
}

void ATPSCharacter::OnSprintToggled(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AbilitySystemComponent->TryActivateAbilityByClass(SprintAbilityClass);
	}
	else
	{
		AbilitySystemComponent->CancelAbility(AbilitySystemComponent->FindAbilitySpecFromClass(SprintAbilityClass)->Ability);
	}
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	auto Attributes = NewObject<UTPSGAPlayerAttributes>(this);
	Attributes->Health.SetBaseValue(30);
	Attributes->Health.SetCurrentValue(30);

	Attributes->MaxHealth.SetBaseValue(30);
	Attributes->MaxHealth.SetCurrentValue(30);
	
	Attributes->Stamina.SetBaseValue(30);
	Attributes->Stamina.SetCurrentValue(30);

	Attributes->MaxStamina.SetBaseValue(30);
	Attributes->MaxStamina.SetCurrentValue(30);
	Attributes->OnDead.AddWeakLambda(this, [this]()
	{
		auto Controller = GetController<APlayerController>();
		Controller->UnPossess();
		UGameplayStatics::GetGameMode(this)->RestartPlayer(Controller);
		Destroy();
	});
	Attributes->OnStaminaOver.AddWeakLambda(this, [this]()
	{
		AbilitySystemComponent->CancelAbility(AbilitySystemComponent->FindAbilitySpecFromClass(SprintAbilityClass)->Ability);
	});
	AbilitySystemComponent->AddSpawnedAttribute(Attributes);
	for (auto Effect : DefaultEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 0, EffectContext);
				
		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
	
	auto Spec = AbilitySystemComponent->BuildAbilitySpecFromClass(SprintAbilityClass);
	AbilitySystemComponent->GiveAbility(Spec);
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ATPSCharacter, EquippedWeaponItem, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ATPSCharacter, EquippedWeaponActor, Params);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Look);
		
		EnhancedInputComponent->BindAction(LeftMouseInputAction, ETriggerEvent::Triggered, this, &ATPSCharacter::LeftMouseAction);
		EnhancedInputComponent->BindAction(RightMouseInputAction, ETriggerEvent::Triggered, this, &ATPSCharacter::RightMouseAction);
		EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Triggered, this, &ATPSCharacter::OnSprintToggled);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}