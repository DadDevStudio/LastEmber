#include "EmberCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AEmberCharacterBase::AEmberCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// === MOVEMENT / CROUCH ===
	auto* Move = GetCharacterMovement();
	Move->NavAgentProps.bCanCrouch = true;
	Move->GetNavAgentPropertiesRef().bCanCrouch = true;
	Move->SetCrouchedHalfHeight(40.f);
	Move->MaxWalkSpeedCrouched = 200.f;

	// === THIRD PERSON MESH (default Mesh component) ===
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;

	// Correct mannequin transform (UE mannequin stands lower & rotated)
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// === THIRD PERSON CAMERA ===
	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	ThirdPersonSpringArm->SetupAttachment(RootComponent);
	ThirdPersonSpringArm->TargetArmLength = 300.f;
	ThirdPersonSpringArm->bUsePawnControlRotation = true;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);

	// === FIRST PERSON CAMERA ===
	// (IMPORTANT: attach to RootComponent, not to Mesh)
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // height of eyes
	FirstPersonCamera->bUsePawnControlRotation = true;

	// === FIRST PERSON MESH ===
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// === DEFAULT VIEW ===
	bIsFirstPerson = true;
	ThirdPersonSpringArm->SetHiddenInGame(true);
	ThirdPersonCamera->SetActive(false);
	FirstPersonCamera->SetActive(true);
}

void AEmberCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		// Hide TP mesh for owner (player sees only FP mesh)
		GetMesh()->SetOwnerNoSee(true);

		// Ensure FP mesh is visible only for owner
		FirstPersonMesh->SetOwnerNoSee(false);
	}
}

void AEmberCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEmberCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	UInputAction* IA_Interact = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Interact.IA_Interact"));
	UInputAction* IA_Inventory = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Inventory.IA_Inventory"));
	UInputAction* IA_UseItem = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_UseItem.IA_UseItem"));
	UInputAction* IA_Fire = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Fire.IA_Fire"));
	UInputAction* IA_ADS = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_ADS.IA_ADS"));
	UInputAction* IA_BuildToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_BuildToggle.IA_BuildToggle"));

	// Move	
	UInputAction* IA_Move = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEmberCharacterBase::Move);

	// Look
	UInputAction* IA_Look = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AEmberCharacterBase::Look);

	// Jump
	UInputAction* IA_Jump = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AEmberCharacterBase::Jump);

	// HOLD Crouch – CTRL
	UInputAction* IA_CrouchHold = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_CrouchHold.IA_CrouchHold"));
	EIC->BindAction(IA_CrouchHold, ETriggerEvent::Started, this, &AEmberCharacterBase::StartCrouch);
	EIC->BindAction(IA_CrouchHold, ETriggerEvent::Completed, this, &AEmberCharacterBase::StopCrouch);

	// TOGGLE Crouch – C
	UInputAction* IA_CrouchToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_CrouchToggle.IA_CrouchToggle"));
	EIC->BindAction(IA_CrouchToggle, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleCrouchMode);

	// HOLD sprint
	UInputAction* IA_SprintHold = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_SprintHold.IA_SprintHold"));
	EIC->BindAction(IA_SprintHold, ETriggerEvent::Started, this, &AEmberCharacterBase::StartSprint);
	EIC->BindAction(IA_SprintHold, ETriggerEvent::Completed, this, &AEmberCharacterBase::StopSprint);

	// TOGGLE sprint
	UInputAction* IA_SprintToggle = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_SprintToggle.IA_SprintToggle"));
	EIC->BindAction(IA_SprintToggle, ETriggerEvent::Started, this, &AEmberCharacterBase::ToggleSprintMode);

	// Toggle Perspective 
	UInputAction* IA_TogglePerspective = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_TogglePerspective.IA_TogglePerspective"));
	EIC->BindAction(IA_TogglePerspective, ETriggerEvent::Started, this, &AEmberCharacterBase::TogglePerspective);

	// Basic interactions
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

void AEmberCharacterBase::TogglePerspective(const FInputActionValue&)
{
	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
		ThirdPersonSpringArm->SetHiddenInGame(true);
	}
	else
	{
		FirstPersonCamera->SetActive(false);
		ThirdPersonCamera->SetActive(true);
		ThirdPersonSpringArm->SetHiddenInGame(false);
	}
}

// ========= EMPTY ACTIONS =========

void AEmberCharacterBase::Interact(const FInputActionValue&) {}
void AEmberCharacterBase::ToggleInventory(const FInputActionValue&) {}
void AEmberCharacterBase::UseItem(const FInputActionValue&) {}
void AEmberCharacterBase::Fire(const FInputActionValue&) {}
void AEmberCharacterBase::ADS(const FInputActionValue&) {}
void AEmberCharacterBase::ToggleBuildMode(const FInputActionValue&) {}
