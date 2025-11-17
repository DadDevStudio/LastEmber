#include "EmberPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

AEmberPlayerController::AEmberPlayerController()
{
	
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextFinder(
	TEXT("/Game/Input/IMC/IMC_Default.IMC_Default") );
	if (MappingContextFinder.Succeeded())
	{
		DefaultMappingContext = MappingContextFinder.Object;
	}
}

void AEmberPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// future: initialize HUD, input contexts, etc.
}

void AEmberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		
	}
	
}