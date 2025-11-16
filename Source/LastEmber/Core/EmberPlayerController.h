#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.generated.h"

UCLASS()
class AEmberPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEmberPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* DefaultMappingContext;
};
