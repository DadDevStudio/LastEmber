// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// 🔥 UWAGA: dodajemy właściwe includy z Enhanced Input
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "LastEmberPlayerController.generated.h"

class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * PlayerController for Last Ember - handles Enhanced Input bindings
 */
UCLASS()
class LASTEMBER_API ALastEmberPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// --- Enhanced Input assets ---
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void HandleJump();
};
