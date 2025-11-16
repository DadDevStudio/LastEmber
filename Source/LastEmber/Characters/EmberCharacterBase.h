#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EmberCharacterBase.generated.h"

UCLASS()
class AEmberCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEmberCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Movement functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	// Crouch
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void ToggleCrouchMode(const FInputActionValue& Value);
	bool bCrouchToggled = false;
	// Sprint
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void ToggleSprintMode(const FInputActionValue& Value);

	bool bSprintToggled = false;
	bool bSprintHeld = false;
	
	void Interact(const FInputActionValue& Value);
	void ToggleInventory(const FInputActionValue& Value);
	void UseItem(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void ADS(const FInputActionValue& Value);
	void ToggleBuildMode(const FInputActionValue& Value);

public:
	virtual void Tick(float DeltaTime) override;
};
