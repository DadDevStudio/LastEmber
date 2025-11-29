// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberAbilitySystemComponent.cpp
#include "GAS/LastEmberAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GAS/Abilities/GA_ConsumeFood.h"
// #include "GA_Heal.h"

void ULastEmberAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		InitializeAbilities();
	}
}

void ULastEmberAbilitySystemComponent::InitializeAbilities()
{
	// Dodajemy zdolności graczowi (np. jedzenie, leczenie)
	GiveAbility(FGameplayAbilitySpec(UGA_ConsumeFood::StaticClass(), 1, 0));
	// GiveAbility(FGameplayAbilitySpec(UGA_Heal::StaticClass(), 1, 1));
}
