// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "LEPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * PlayerState that owns the AbilitySystemComponent and AttributeSet (GAS pattern)
 */
UCLASS()
class LASTEMBER_API ALEPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALEPlayerState();

	// IAbilitySystemInterface implementation
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;
	// ASC & Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAttributeSet* Attributes; // konkretny typ (Twoj AttributeSet) możesz ustawić w cpp konstruktorze

};
