// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LECharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class LASTEMBER_API ALECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALECharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* Camera1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* Mesh1P;
	
	// Movement/Input functions
	void Move(const FVector2D& Input);
	void Look(const FVector2D& Input);
	void StartJump();
	void StopJump();
	
	UFUNCTION(Exec, BlueprintCallable, Category="Debug")
	void SetLookSensitivity(float NewSensitivity);
	
	// --- Tunables (exposed to BP for easy tweaking) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	float LookSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	bool bInvertLookY = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	bool bSwapMoveAxes = true; // if true: interpret Input.Y as forward, Input.X as right
	
	
};
