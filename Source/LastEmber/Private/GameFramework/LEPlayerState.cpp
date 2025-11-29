// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#include "GameFramework/LEPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS/LEAttributeSet.h"
#include "Net/UnrealNetwork.h"

ALEPlayerState::ALEPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	}

	Attributes = CreateDefaultSubobject<ULEAttributeSet>(TEXT("Attributes"));

	UE_LOG(LogTemp, Warning, TEXT("[GAS][PlayerState] Constructor called on %s (IsNetModeClient=%d)"), *GetName(), IsNetMode(NM_Client));
}

void ALEPlayerState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[GAS][PlayerState] BeginPlay on %s (ActorRole=%d, RemoteRole=%d)"), *GetName(), (int)GetLocalRole(), (int)GetRemoteRole());
}
UAbilitySystemComponent* ALEPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}