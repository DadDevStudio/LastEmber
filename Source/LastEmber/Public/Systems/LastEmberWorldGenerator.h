// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberWorldGenerator.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LastEmberWorldGenerator.generated.h"

UCLASS()
class LASTEMBER_API ALastEmberWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	ALastEmberWorldGenerator();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
