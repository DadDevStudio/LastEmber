#include "EmberGameMode.h"

#include "EmberHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.h"
#include "Characters/EmberCharacterBase.h"
#include "EmberPlayerState.h"

AEmberGameMode::AEmberGameMode()
{
	// Use our custom PlayerController and PlayerState
	PlayerControllerClass = AEmberPlayerController::StaticClass();
	PlayerStateClass = AEmberPlayerState::StaticClass();
	HUDClass = AEmberHUD::StaticClass();
	
	// Default pawn class will be set in constructor if EmberCharacterBase is available
	DefaultPawnClass = AEmberCharacterBase::StaticClass();
}
