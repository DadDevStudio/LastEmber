// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberPlayerController.cpp

#include "Core/LastEmberPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/LastEmberCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ALastEmberPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (PlayerMappingContext)
        {
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
            UE_LOG(LogTemp, Log, TEXT("✅ MappingContext applied to PlayerController"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ PlayerMappingContext not set!"));
        }
    }
}
void ALastEmberPlayerController::HandleJump()
{
    if (ACharacter* ControlledChar = Cast<ACharacter>(GetPawn()))
    {
        ControlledChar->Jump();
    }
}
void ALastEmberPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction)
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALastEmberPlayerController::Move);

        if (LookAction)
            EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALastEmberPlayerController::Look);

        if (JumpAction)
            EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ALastEmberPlayerController::HandleJump);
        if (TogglePerspectiveAction)
        {
            EIC->BindAction(TogglePerspectiveAction, ETriggerEvent::Started, this, &ALastEmberPlayerController::HandleTogglePerspective);
        }
    }
}


void ALastEmberPlayerController::Move(const FInputActionValue& Value)
{
    const FVector2D MoveVector = Value.Get<FVector2D>();
    if (APawn* ControlledPawn = GetPawn())
    {
        const FRotator Rotation = ControlledPawn->GetActorRotation();
        const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

        ControlledPawn->AddMovementInput(Forward, MoveVector.Y);
        ControlledPawn->AddMovementInput(Right, MoveVector.X);
    }
}

void ALastEmberPlayerController::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxis = Value.Get<FVector2D>();
    this->AddYawInput(LookAxis.X);   // użyj "this->" lub upewnij się, że metoda jest członkiem klasy
    this->AddPitchInput(LookAxis.Y);
}

void ALastEmberPlayerController::HandleTogglePerspective()
{
    if (ALastEmberCharacterBase* Char = Cast<ALastEmberCharacterBase>(GetPawn()))
    {
        Char->ToggleCameraView(); // to funkcja z wcześniejszego kroku, która przełącza kamerę
    }
}