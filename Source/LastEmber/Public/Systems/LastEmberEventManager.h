// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberEventManager.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LastEmberEventManager.generated.h"

UCLASS()
class LASTEMBER_API ALastEmberEventManager : public AActor
{
	GENERATED_BODY()

public:
	ALastEmberEventManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
