// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#include "LastEmber/Public/GameFramework/LEPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Constructor
ALEPlayerController::ALEPlayerController()
{
    UE_LOG(LogTemp, Warning, TEXT("LEPlayerController Constructor called!"));
}

// Called when the game starts or when spawned
void ALEPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("LEPlayerController BeginPlay called!"));

    // Get the Enhanced Input Local Player Subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Add the Input Mapping Context
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
        UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context added!"));
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
        
        UE_LOG(LogTemp, Warning, TEXT("Input Actions bound!"));
    }
}

// Move Input
void ALEPlayerController::Move(const FInputActionValue& Value)
{
    // Get the 2D Axis value (forward/right)
    const FVector2D MovementVector = Value.Get<FVector2D>();
    
    UE_LOG(LogTemp, Warning, TEXT("Move: X=%.2f, Y=%.2f"), MovementVector.X, MovementVector.Y);
}

// Look Input
void ALEPlayerController::Look(const FInputActionValue& Value)
{
    // Get the 2D Axis value (mouse X/Y)
    const FVector2D LookVector = Value.Get<FVector2D>();
    
    UE_LOG(LogTemp, Warning, TEXT("Look: X=%.2f, Y=%.2f"), LookVector.X, LookVector.Y);
}

// Jump Input
void ALEPlayerController::Jump()
{
    UE_LOG(LogTemp, Warning, TEXT("Jump pressed!"));
}

// Shoot Input
void ALEPlayerController::Shoot()
{
    UE_LOG(LogTemp, Warning, TEXT("Shoot pressed!"));
}