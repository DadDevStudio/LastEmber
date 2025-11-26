// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmber/Public/GameFramework/LEGameMode.h"

#include "LastEmber/Public/GameFramework/LEPlayerController.h"

ALEGameMode::ALEGameMode()
{
	//Set Default PlayerController class
	PlayerControllerClass = ALEPlayerController::StaticClass();
	
	UE_LOG(LogTemp, Warning, TEXT("LEGameMode Constructor called!"));
}

// Called when game starts
void ALEGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("LEGameMode BeginPlay called!"));
}
