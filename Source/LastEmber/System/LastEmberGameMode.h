// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LastEmberGameMode.generated.h"

UCLASS()
class LASTEMBER_API ALastEmberGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Funkcja, którą wywoła Postać (lub Controller) po śmierci, z prośbą o respawn
	void RequestRespawn(AController* Controller, float Delay);

protected:
	// Wewnętrzna funkcja, która wykona się po upływie czasu (Delay)
	void HandleRespawnDelay(AController* Controller);
};