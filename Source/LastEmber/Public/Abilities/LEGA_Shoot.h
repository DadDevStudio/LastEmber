// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LEGA_Shoot.generated.h"

class UGameplayEffect;

/**
 * Simple hitscan shoot ability.
 * - Draws debug line/sphere
 * - On server: applies DamageGameplayEffect via SetByCaller "Damage" tag
 */
UCLASS()
class LASTEMBER_API ULEGA_Shoot : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULEGA_Shoot();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Trace")
	float TraceRange = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	float BaseDamage = 25.f;

	// Optional: assign GE asset in editor (GE should use SetByCaller "Damage")
	UPROPERTY(EditDefaultsOnly, Category="Ability|Damage")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;
};
