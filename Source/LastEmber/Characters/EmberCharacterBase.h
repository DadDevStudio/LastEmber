#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EmberCharacterBase.generated.h"

UCLASS()
class AEmberCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEmberCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
