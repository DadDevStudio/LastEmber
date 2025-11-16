#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EmberMovementComponent.generated.h"

UCLASS()
class UEmberMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UEmberMovementComponent();

	// future: custom movement properties and replication helpers
};
