// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LastEmberInteractable.generated.h"

// To jest tylko "formułka" dla Unreala, nie ruszamy tego
UINTERFACE(MinimalAPI)
class ULastEmberInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * To jest właściwy interfejs.
 */
class LASTEMBER_API ILastEmberInteractable
{
	GENERATED_BODY()

public:
	// Funkcja, którą każdy przedmiot (Drzwi, Jedzenie) musi zaimplementować po swojemu.
	// BlueprintNativeEvent = Możemy napisać logikę w C++ LUB w Blueprincie (super elastyczne).
	// Instigator = Kto nacisnął przycisk (zazwyczaj my).
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APawn* InstigatorPawn);
};