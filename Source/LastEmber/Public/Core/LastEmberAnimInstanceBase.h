// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LastEmberAnimInstanceBase.generated.h"

/**
 * Thread-safe base anim instance used by both FP and TP characters.
 * Handles animation variables and exposes sync interface.
 */
UCLASS()
class LASTEMBER_API ULastEmberAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Movement speed (updated in AnimGraph thread)
	UPROPERTY(BlueprintReadOnly, Category="AnimData", meta=(AllowPrivateAccess="true"))
	float Speed = 0.f;

	// Whether character is in air
	UPROPERTY(BlueprintReadOnly, Category="AnimData", meta=(AllowPrivateAccess="true"))
	bool bIsInAir = false;

	// Whether character is aiming (ADS)
	UPROPERTY(BlueprintReadOnly, Category="AnimData", meta=(AllowPrivateAccess="true"))
	bool bIsAiming = false;

	// Sync animation (like reload or melee)
	UFUNCTION(BlueprintCallable, Category="Animation")
	virtual void PlaySharedMontage(UAnimMontage* Montage);
};
