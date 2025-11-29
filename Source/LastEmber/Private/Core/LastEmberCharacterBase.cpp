// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberCharacterBase.cpp
#include "Core/LastEmberCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GAS/LastEmberSurvivalComponent.h"
// #include "GAS/LastEmberInventoryComponent.h"
#include "Net/UnrealNetwork.h"

ALastEmberCharacterBase::ALastEmberCharacterBase()
{
	bReplicates = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	SurvivalComponent = CreateDefaultSubobject<ULastEmberSurvivalComponent>("Survival");
	// InventoryComponent = CreateDefaultSubobject<ULastEmberInventoryComponent>("Inventory");
}

void ALastEmberCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		UE_LOG(LogTemp, Log, TEXT("Character initialized on server"));
}

void ALastEmberCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Replikacja niestandardowych zmiennych gdy dodamy survival data
}
