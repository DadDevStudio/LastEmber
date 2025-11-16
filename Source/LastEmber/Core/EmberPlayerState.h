#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EmberPlayerState.generated.h"

UCLASS()
class AEmberPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEmberPlayerState();

	// Example: persistent player data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ember")
	int32 PlayerLevel = 1;
};
