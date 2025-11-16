#include "EmberPlayerController.h"

AEmberPlayerController::AEmberPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void AEmberPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// future: initialize HUD, input contexts, etc.
}
