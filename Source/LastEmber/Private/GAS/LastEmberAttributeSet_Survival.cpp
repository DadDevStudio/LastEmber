// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberAttributeSet_Survival.cpp
#include "GAS/LastEmberAttributeSet_Survival.h"
#include "Net/UnrealNetwork.h"

void ULastEmberAttributeSet_Survival::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Hunger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Thirst, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Stamina, COND_None, REPNOTIFY_Always);
}

void ULastEmberAttributeSet_Survival::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Health, OldValue);
}

void ULastEmberAttributeSet_Survival::OnRep_Hunger(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Hunger, OldValue);
}

void ULastEmberAttributeSet_Survival::OnRep_Thirst(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Thirst, OldValue);
}

void ULastEmberAttributeSet_Survival::OnRep_Stamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Stamina, OldValue);
}
