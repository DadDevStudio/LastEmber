// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberSurvivalComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LastEmberSurvivalComponent.generated.h"

class UAbilitySystemComponent;
class ULastEmberAttributeSet_Survival;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LASTEMBER_API ULastEmberSurvivalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULastEmberSurvivalComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY()
	ULastEmberAttributeSet_Survival* Attributes;

	float HungerRate = 0.05f;
	float ThirstRate = 0.08f;

	void ApplyDamageIfNeeded();
};
