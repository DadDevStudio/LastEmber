// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmber/Public/GameFramework/LEGameMode.h"

#include "GameplayAbilitySpec.h"
#include "Abilities/LEGA_Shoot.h"
#include "GameFramework/LEPlayerState.h"
#include "AbilitySystemComponent.h"
#include "LastEmber/Public/GameFramework/LEPlayerController.h"

ALEGameMode::ALEGameMode()
{
	//Set Default PlayerController class
	PlayerControllerClass = ALEPlayerController::StaticClass();
	PlayerStateClass = ALEPlayerState::StaticClass();
	
}

// Called when game starts
void ALEGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
void ALEGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!NewPlayer) return;

	if (APlayerState* PSBase = NewPlayer->PlayerState)
	{
		if (ALEPlayerState* PS = Cast<ALEPlayerState>(PSBase))
		{
			if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
			{
				ASC->GiveAbility(FGameplayAbilitySpec(ULEGA_Shoot::StaticClass(), 1, INDEX_NONE, PS));
				UE_LOG(LogTemp, Warning, TEXT("[GAMEMODE] Gave Shoot ability to %s"), *PS->GetPlayerName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[GAMEMODE] PlayerState has no ASC for %s"), *PS->GetPlayerName());
			}
		}
	}
}