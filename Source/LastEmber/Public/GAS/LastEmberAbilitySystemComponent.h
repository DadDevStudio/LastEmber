// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberAbilitySystemComponent.h
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LastEmberAbilitySystemComponent.generated.h"

UCLASS()
class LASTEMBER_API ULastEmberAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void InitializeAbilities();

protected:
	void BeginPlay() override;
};
