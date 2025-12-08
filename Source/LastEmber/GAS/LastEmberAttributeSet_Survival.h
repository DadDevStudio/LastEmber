// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LastEmberAttributeSet_Survival.generated.h"

// Makro pomocnicze (Boilerplate) - generuje gettery, settery i initery
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class LASTEMBER_API ULastEmberAttributeSet_Survival : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULastEmberAttributeSet_Survival();

	// Konfiguracja replikacji sieciowej
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Funkcja wywoływana PRZED zmianą wartości atrybutu (np. przy leczeniu).
	// Tutaj pilnujemy, żeby HP nie przekroczyło MaxHP, a Głód nie spadł poniżej 0.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Funkcja wywoływana PO nałożeniu Gameplay Effectu (np. otrzymanie obrażeń).
	// Używana do logiki typu "Jeśli HP spadło do 0, odpal funkcję Śmierć".
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ==================================================
	// 1. ZDROWIE (Health)
	// ==================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Survival")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Survival")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, MaxHealth);

	// ==================================================
	// 2. GŁÓD (Hunger)
	// 100 = Najedzony, 0 = Śmierć głodowa
	// ==================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Hunger, Category = "Attributes|Survival")
	FGameplayAttributeData Hunger;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, Hunger);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHunger, Category = "Attributes|Survival")
	FGameplayAttributeData MaxHunger;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, MaxHunger);

	// ==================================================
	// 3. PRAGNIENIE (Thirst)
	// 100 = Nawodniony, 0 = Śmierć z pragnienia
	// ==================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Thirst, Category = "Attributes|Survival")
	FGameplayAttributeData Thirst;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, Thirst);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxThirst, Category = "Attributes|Survival")
	FGameplayAttributeData MaxThirst;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, MaxThirst);

	// ==================================================
	// 4. ZAKAŻENIE (Infection) - a'la Undead Legacy
	// 0 = Zdrowy, 100 = Przemiana w zombie/Śmierć
	// ==================================================
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Infection, Category = "Attributes|Survival")
	FGameplayAttributeData Infection;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, Infection);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxInfection, Category = "Attributes|Survival")
	FGameplayAttributeData MaxInfection;
	ATTRIBUTE_ACCESSORS(ULastEmberAttributeSet_Survival, MaxInfection);

protected:
	// Funkcje OnRep - powiadamiają UI klienta o zmianach
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Hunger(const FGameplayAttributeData& OldHunger);
	UFUNCTION()
	virtual void OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger);

	UFUNCTION()
	virtual void OnRep_Thirst(const FGameplayAttributeData& OldThirst);
	UFUNCTION()
	virtual void OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst);

	UFUNCTION()
	virtual void OnRep_Infection(const FGameplayAttributeData& OldInfection);
	UFUNCTION()
	virtual void OnRep_MaxInfection(const FGameplayAttributeData& OldMaxInfection);
};