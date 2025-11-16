#include "EmberCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"

AEmberCharacterBase::AEmberCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	auto* Move = GetCharacterMovement();
	Move->NavAgentProps.bCanCrouch = true;
	Move->GetNavAgentPropertiesRef().bCanCrouch = true;

	Move->SetCrouchedHalfHeight(40.f);
	Move->MaxWalkSpeedCrouched = 200.f;
	// GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	// Optionally: set a custom movement component class later
	// GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEmberCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEmberCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AEmberCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    // Load assets
    UInputAction* IA_Move = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
    UInputAction* IA_Look = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
    UInputAction* IA_Jump = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	UInputAction* IA_CrouchHold = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_CrouchHold.IA_CrouchHold"));
	UInputAction* IA_CrouchToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_CrouchToggle.IA_CrouchToggle"));
	// UInputAction* IA_Crouch = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Crouch.IA_Crouch"));
    UInputAction* IA_Interact = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Interact.IA_Interact"));
    UInputAction* IA_Inventory = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Inventory.IA_Inventory"));
    UInputAction* IA_UseItem = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_UseItem.IA_UseItem"));
    UInputAction* IA_Fire = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Fire.IA_Fire"));
    UInputAction* IA_ADS = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_ADS.IA_ADS"));
    UInputAction* IA_BuildToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_BuildToggle.IA_BuildToggle"));
	// Sprints
	UInputAction* IA_SprintHold = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_SprintHold.IA_SprintHold"));
	UInputAction* IA_SprintToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_SprintToggle.IA_SprintToggle"));

    // Binds
    EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEmberCharacterBase::Move);
    EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AEmberCharacterBase::Look);
    EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AEmberCharacterBase::Jump);
	// HOLD Crouch – CTRL
	EIC->BindAction(IA_CrouchHold, ETriggerEvent::Started,  this, &AEmberCharacterBase::StartCrouch);
	EIC->BindAction(IA_CrouchHold, ETriggerEvent::Completed, this, &AEmberCharacterBase::StopCrouch);

	// TOGGLE Crouch – C
	EIC->BindAction(IA_CrouchToggle, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleCrouchMode);

	// HOLD sprint
	EIC->BindAction(IA_SprintHold, ETriggerEvent::Started, this, &AEmberCharacterBase::StartSprint);
	EIC->BindAction(IA_SprintHold, ETriggerEvent::Completed, this, &AEmberCharacterBase::StopSprint);

	// TOGGLE sprint
	EIC->BindAction(IA_SprintToggle, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleSprintMode);

    EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &AEmberCharacterBase::Interact);
    EIC->BindAction(IA_Inventory, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleInventory);
    EIC->BindAction(IA_UseItem, ETriggerEvent::Started, this, &AEmberCharacterBase::UseItem);
    EIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AEmberCharacterBase::Fire);
    EIC->BindAction(IA_ADS, ETriggerEvent::Triggered, this, &AEmberCharacterBase::ADS);
    EIC->BindAction(IA_BuildToggle, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleBuildMode);
}
void AEmberCharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Input.Y);
	AddMovementInput(GetActorRightVector(), Input.X);
}

void AEmberCharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void AEmberCharacterBase::StartCrouch(const FInputActionValue&)
{
	Crouch();
}
void AEmberCharacterBase::StopCrouch(const FInputActionValue&)
{
	UnCrouch();
}
void AEmberCharacterBase::ToggleCrouchMode(const FInputActionValue&)
{
	bCrouchToggled = !bCrouchToggled;

	if (bCrouchToggled)
		Crouch();
	else
		UnCrouch();
}
void AEmberCharacterBase::StartSprint(const FInputActionValue&)
{
	bSprintHeld = true;

	// If toggle sprint was active → disable it
	if (bSprintToggled)
	{
		bSprintToggled = false;
	}

	// If crouched → stand up
	if (bIsCrouched)
		UnCrouch();

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}
void AEmberCharacterBase::StopSprint(const FInputActionValue&)
{
	bSprintHeld = false;

	// toggle sprint was cleared when Shift was pressed,
	// so return to normal speed
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}
void AEmberCharacterBase::ToggleSprintMode(const FInputActionValue&)
{
	bSprintToggled = !bSprintToggled;

	// If crouched → stand
	if (bIsCrouched)
		UnCrouch();

	if (bSprintToggled)
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
	else
	{
		// If player is holding Shift, keep sprint
		if (!bSprintHeld)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
	}
}

void AEmberCharacterBase::Interact(const FInputActionValue&) {}
void AEmberCharacterBase::ToggleInventory(const FInputActionValue&) {}
void AEmberCharacterBase::UseItem(const FInputActionValue&) {}
void AEmberCharacterBase::Fire(const FInputActionValue&) {}
void AEmberCharacterBase::ADS(const FInputActionValue&) {}
void AEmberCharacterBase::ToggleBuildMode(const FInputActionValue&) {}
