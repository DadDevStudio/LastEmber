// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// GA_ConsumeFood.cpp
#include "GAS/Abilities/GA_ConsumeFood.h"
#include "GAS/LastEmberAttributeSet_Survival.h"
#include "AbilitySystemComponent.h"

UGA_ConsumeFood::UGA_ConsumeFood()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ConsumeFood::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
									  const FGameplayAbilityActorInfo* ActorInfo,
									  const FGameplayAbilityActivationInfo ActivationInfo,
									  const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	auto* Attr = const_cast<ULastEmberAttributeSet_Survival*>(ASC->GetSet<ULastEmberAttributeSet_Survival>());

	if (Attr)
	{
		Attr->SetHunger(FMath::Clamp(Attr->GetHunger() + 30.f, 0.f, 100.f));
		Attr->SetThirst(FMath::Clamp(Attr->GetThirst() + 20.f, 0.f, 100.f));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
