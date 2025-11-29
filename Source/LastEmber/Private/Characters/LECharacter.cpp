// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "Characters/LECharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/LEPlayerState.h"
#include "AbilitySystemComponent.h"


// Sets default values
ALECharacter::ALECharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Mesh 1st Person
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	Mesh1P->SetupAttachment(GetMesh());
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	
	// Camera 1st Person
	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera1P->SetupAttachment(Mesh1P);
	Camera1P->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.f));
	Camera1P->bUsePawnControlRotation = true;
	
	
}

void ALECharacter::Move(const FVector2D& Input)
{

	// current camera based directions (jak wcześniej)
	FVector CamForward = Camera1P->GetForwardVector();
	FVector CamRight   = Camera1P->GetRightVector();
	CamForward.Z = 0.f; CamRight.Z = 0.f;
	CamForward.Normalize(); CamRight.Normalize();

	// DODATKOWE: wypisz rotacje do debugu
	const FRotator CamRot = Camera1P->GetComponentRotation();
	const FRotator CtrlRot = Controller->GetControlRotation();
	const FRotator ActorRot = GetActorRotation();


	// dalej normalnie: interpretacja input i AddMovementInput
	const float ForwardValue = bSwapMoveAxes ? Input.Y : Input.X;
	const float RightValue   = bSwapMoveAxes ? Input.X : Input.Y;

	if (!FMath::IsNearlyZero(ForwardValue))
		AddMovementInput(CamForward, ForwardValue);
	if (!FMath::IsNearlyZero(RightValue))
		AddMovementInput(CamRight, RightValue);
}




void ALECharacter::Look(const FVector2D& Input)
{

	const float YawValue = Input.X * LookSensitivity;
	const float PitchRaw = Input.Y;
	const float PitchValue = (bInvertLookY ? -PitchRaw : PitchRaw) * LookSensitivity;

	AddControllerYawInput(YawValue);
	AddControllerPitchInput(PitchValue);
}

void ALECharacter::SetLookSensitivity(float NewSensitivity)
{
	LookSensitivity = NewSensitivity;
}

void ALECharacter::StartJump()
{
	Jump();
}

void ALECharacter::StopJump()
{
	StopJumping();
}

// Called when the game starts or when spawned
void ALECharacter::BeginPlay()
{
	Super::BeginPlay();
	// Jeśli PlayerState ma ASC, zainicjalizuj AbilityActorInfo (PS = owner, this = avatar)
	if (APlayerState* PSBase = GetPlayerState())
	{
		if (ALEPlayerState* PS = Cast<ALEPlayerState>(PSBase))
		{
			if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
			{
				ASC->InitAbilityActorInfo(PS, this);
				UE_LOG(LogTemp, Warning, TEXT("GAS: AbilityActorInfo initialized for %s"), *GetName());
			}
		}
	}
}

// Called every frame
void ALECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

