// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberGameState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LastEmberGameState.generated.h"

UCLASS()
class LASTEMBER_API ALastEmberGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALastEmberGameState();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_TimeOfDay)
	float TimeOfDay;

	UPROPERTY(Replicated)
	FString CurrentWeather;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_TimeOfDay();

	void Tick(float DeltaTime) override;
};
