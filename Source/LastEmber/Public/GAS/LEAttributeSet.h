// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "LEAttributeSet.generated.h"

UCLASS()
class LASTEMBER_API ULEAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULEAttributeSet();

	// Health - replikujemy z notify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;

	// OnRep handler required for GAS attribute replication
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	// Ręczne gettery/settery
	FORCEINLINE float GetHealth() const { return Health.GetCurrentValue(); }
	FORCEINLINE void SetHealth(float NewVal) { Health.SetCurrentValue(NewVal); }
	FORCEINLINE FGameplayAttribute GetHealthAttribute() const
	{
		return FGameplayAttribute(FindFieldChecked<FProperty>(ULEAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(ULEAttributeSet, Health)));
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
