// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"  // ← DODAJ TO!
#include "LEPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * Player Controller for Last Ember
 * Handles player input, camera control, and UI
 */
UCLASS()
class LASTEMBER_API ALEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Constructor
	ALEPlayerController();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Setup Input Component (Enhanced Input)
	virtual void SetupInputComponent() override;
	
	// Enhanced Input - Mapping Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
	// Enhanced Input - Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* ShootAction;
	
	// Input callbacks
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Shoot();
};
