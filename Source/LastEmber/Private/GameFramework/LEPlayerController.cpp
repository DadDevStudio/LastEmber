// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#include "LastEmber/Public/GameFramework/LEPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/LECharacter.h"

// Constructor
ALEPlayerController::ALEPlayerController()
{
}

// Called when the game starts or when spawned
void ALEPlayerController::BeginPlay()
{
    Super::BeginPlay();
    

    // Get the Enhanced Input Local Player Subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Add the Input Mapping Context
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}
      
// Setup Input Component (Enhanced Input)
void ALEPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Cast to Enhanced Input Component
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind Move action
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALEPlayerController::Move);
        
        // Bind Look action
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALEPlayerController::Look);
        
        // Bind Jump action
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALEPlayerController::Jump);
        
        // Bind Shoot action
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ALEPlayerController::Shoot);
        
    }
}

// Move Input

void ALEPlayerController::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    APawn* P = GetPawn();

    if (P)
    {
        if (ALECharacter* Ch = Cast<ALECharacter>(P))
        {
            Ch->Move(MovementVector);
            return;
        }
        // fallback
        P->AddMovementInput(P->GetActorForwardVector(), MovementVector.X);
        P->AddMovementInput(P->GetActorRightVector(), MovementVector.Y);
    }
}


void ALEPlayerController::Look(const FInputActionValue& Value)
{
    if (ALECharacter* Ch = Cast<ALECharacter>(GetPawn()))
        Ch->Look(Value.Get<FVector2D>());
}

void ALEPlayerController::Jump()
{
    if (ALECharacter* Ch = Cast<ALECharacter>(GetPawn()))
        Ch->StartJump();
}

void ALEPlayerController::Shoot()  // zostawimy na później
{
}