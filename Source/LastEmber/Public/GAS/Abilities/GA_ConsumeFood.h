// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// GA_ConsumeFood.h
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ConsumeFood.generated.h"

UCLASS()
class LASTEMBER_API UGA_ConsumeFood : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ConsumeFood();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
};
