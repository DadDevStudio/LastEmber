// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberCharacterBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LastEmberCharacterBase.generated.h"

class ULastEmberSurvivalComponent;
class ULastEmberInventoryComponent;
class UAbilitySystemComponent;

UCLASS()
class LASTEMBER_API ALastEmberCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALastEmberCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULastEmberSurvivalComponent* SurvivalComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// ULastEmberInventoryComponent* InventoryComponent;
};
