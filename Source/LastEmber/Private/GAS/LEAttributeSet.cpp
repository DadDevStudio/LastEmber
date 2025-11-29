#include "GAS/LEAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

// Konstruktor
ULEAttributeSet::ULEAttributeSet()
{
	Health = 100.f;
}

// Repl. props
void ULEAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replikuj Health i wywołaj OnRep (REPNOTIFY_Always sprawia że OnRep jest wywołany nawet jak wartość ta sama)
	DOREPLIFETIME_CONDITION_NOTIFY(ULEAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

// OnRep handler — używamy macro GAS do notyfikacji widoków/blueprintów
void ULEAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	// To makro powiadomi bindingi GAS/blueprint i wywoła OnRep w atrybutach
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULEAttributeSet, Health, OldValue);
}

void ULEAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Przykład: clamp health
	float NewHealth = Health.GetCurrentValue();
	NewHealth = FMath::Clamp(NewHealth, 0.0f, 999999.f);
	Health.SetCurrentValue(NewHealth);
}
