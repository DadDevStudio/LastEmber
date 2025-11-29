// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberSurvivalComponent.cpp
#include "GAS/LastEmberSurvivalComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS/LastEmberAttributeSet_Survival.h"
#include "GameplayEffect.h"

ULastEmberSurvivalComponent::ULastEmberSurvivalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULastEmberSurvivalComponent::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystem = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	Attributes = const_cast<ULastEmberAttributeSet_Survival*>(AbilitySystem->GetSet<ULastEmberAttributeSet_Survival>());
	
}

void ULastEmberSurvivalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AbilitySystem || !Attributes || !GetOwner()->HasAuthority()) return;

	float NewHunger = Attributes->GetHunger() - HungerRate * DeltaTime;
	float NewThirst = Attributes->GetThirst() - ThirstRate * DeltaTime;

	Attributes->SetHunger(NewHunger);
	Attributes->SetThirst(NewThirst);

	ApplyDamageIfNeeded();
}

void ULastEmberSurvivalComponent::ApplyDamageIfNeeded()
{
	if (Attributes->GetHunger() <= 0 || Attributes->GetThirst() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying starvation damage..."));
		float NewHealth = Attributes->GetHealth() - 1.0f;
		Attributes->SetHealth(FMath::Clamp(NewHealth, 0.f, 100.f));
	}
}
