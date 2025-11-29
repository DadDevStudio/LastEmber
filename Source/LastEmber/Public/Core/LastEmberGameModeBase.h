// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberGameModeBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LastEmberGameModeBase.generated.h"

class ALastEmberEventManager;
class ALastEmberWorldGenerator;

UCLASS()
class LASTEMBER_API ALastEmberGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALastEmberGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	ALastEmberEventManager* EventManager;

	UPROPERTY()
	ALastEmberWorldGenerator* WorldGenerator;

	void SpawnWorldSystems();
};
