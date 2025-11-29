// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberGameModeBase.cpp
#include "Core/LastEmberGameModeBase.h"

#include "Core/LastEmberCharacterBase.h"
#include "Core/LastEmberPlayerController.h"
#include "Systems/LastEmberWorldGenerator.h"
#include "Systems/LastEmberEventManager.h"
#include "Kismet/GameplayStatics.h"

ALastEmberGameModeBase::ALastEmberGameModeBase()
{
	// Serwer tylko tutaj — authority model
	DefaultPawnClass = ALastEmberCharacterBase::StaticClass();
	PlayerControllerClass = ALastEmberPlayerController::StaticClass();
	bUseSeamlessTravel = true;
	
}

void ALastEmberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // Only server executes
	{
		UE_LOG(LogTemp, Log, TEXT("Last Ember: Initializing World Systems..."));
		SpawnWorldSystems();
	}
}

void ALastEmberGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("Player joined the world."));
}

void ALastEmberGameModeBase::SpawnWorldSystems()
{
	UWorld* World = GetWorld();
	if (!World) return;

	WorldGenerator = World->SpawnActor<ALastEmberWorldGenerator>();
	EventManager   = World->SpawnActor<ALastEmberEventManager>();
}
