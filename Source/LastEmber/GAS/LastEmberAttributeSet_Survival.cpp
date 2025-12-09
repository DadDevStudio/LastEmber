// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmberAttributeSet_Survival.h"
#include "Net/UnrealNetwork.h"
#include "LastEmber/Characters/LastEmberCharacter.h"
#include "GameplayEffectExtension.h" // Potrzebne do obsługi PostGameplayEffectExecute

ULastEmberAttributeSet_Survival::ULastEmberAttributeSet_Survival()
{
	// Domyślne wartości startowe
	InitHealth(100.0f);
	InitMaxHealth(100.0f);

	InitHunger(100.0f);    // Startujemy najedzeni
	InitMaxHunger(100.0f);

	InitThirst(100.0f);    // Startujemy nawodnieni
	InitMaxThirst(100.0f);

	InitInfection(0.0f);   // Startujemy zdrowi (brak wirusa)
	InitMaxInfection(100.0f);
}

void ULastEmberAttributeSet_Survival::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Rejestracja zmiennych do sieci. 
	// REPNOTIFY_Always zapewnia, że UI odświeży się nawet przy rollbacku serwera.
	
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Hunger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, MaxHunger, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Thirst, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, MaxThirst, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, Infection, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULastEmberAttributeSet_Survival, MaxInfection, COND_None, REPNOTIFY_Always);
}

// --- CLAMPING (Pilnowanie wartości min/max) ---

void ULastEmberAttributeSet_Survival::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Jeśli zmienia się Zdrowie -> pilnuj przedziału 0 do MaxHealth
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	// Jeśli zmienia się Głód -> pilnuj przedziału 0 do MaxHunger
	else if (Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHunger());
	}
	// Jeśli zmienia się Pragnienie -> pilnuj przedziału 0 do MaxThirst
	else if (Attribute == GetThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxThirst());
	}
	// Jeśli zmienia się Infekcja -> pilnuj przedziału 0 do MaxInfection
	else if (Attribute == GetInfectionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxInfection());
	}
}

void ULastEmberAttributeSet_Survival::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Ta funkcja służy do dodatkowej logiki PO nałożeniu efektu.
	// Tutaj też warto robić Clamp na wartości końcowej, bo PreAttributeChange 
	// działa na "Base Value", a niekoniecznie na modyfikatorach tymczasowych.

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		if (GetHealth() <= 0.0f)
		{
			// TODO: Tutaj w przyszłości wywołamy funkcję: HandleDeath();
			// UE_LOG(LogTemp, Warning, TEXT("Postać umarła!"));
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(), 0.0f, GetMaxHunger()));
	}
	else if (Data.EvaluatedData.Attribute == GetThirstAttribute())
	{
		SetThirst(FMath::Clamp(GetThirst(), 0.0f, GetMaxThirst()));
	}
	else if (Data.EvaluatedData.Attribute == GetInfectionAttribute())
	{
		SetInfection(FMath::Clamp(GetInfection(), 0.0f, GetMaxInfection()));
	}
	// --- Logika Zdrowia ---
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clampujemy HP (żeby nie było -50)
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		// SPRAWDZAMY CZY UMARŁ
		if (GetHealth() <= 0.0f)
		{
			// Pobieramy aktora, który posiada te atrybuty
			AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			
			// Rzutujemy na naszą postać
			ALastEmberCharacter* MyChar = Cast<ALastEmberCharacter>(TargetActor);
			
			// Jeśli to nasza postać i jeszcze nie wykonaliśmy procedury śmierci...
			if (MyChar)
			{
				MyChar->HandleDeath();
			}
		}
	}
	
	// --- Logika Głodu (Śmierć głodowa) ---
	else if (Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(), 0.0f, GetMaxHunger()));

		// Jeśli głód spadł do 0 -> Zabieramy HP
		if (GetHunger() <= 0.0f)
		{
			// Tutaj stosujemy "sztuczkę". Zamiast odpalać GameplayEffect, 
			// możemy bezpośrednio odjąć zdrowie w kodzie dla uproszczenia,
			// chociaż profesjonalnie powinno się nałożyć efekt "Starvation Damage".
			
			// Wersja prosta (Direct Modification):
			// Odejmujemy 1 HP przy każdej aktualizacji (czyli co sekundę, bo HungerDecay tyka co sekundę)
			const float StarvationDamage = 5.0f;
			SetHealth(FMath::Clamp(GetHealth() - StarvationDamage, 0.0f, GetMaxHealth()));
			
			// Jeśli to odejmowanie zabiło gracza, musimy ręcznie wywołać sprawdzenie śmierci,
			// bo zmiana wartości przez Setter nie zawsze wywołuje PostGameplayEffectExecute dla Health.
			if (GetHealth() <= 0.0f)
			{
				AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
				ALastEmberCharacter* MyChar = Cast<ALastEmberCharacter>(TargetActor);
				if (MyChar) MyChar->HandleDeath();
			}
		}
	}
	// ... reszta kodu (Thirst, Infection) ...
}

// --- BOILERPLATE REPLIKACJI ---

void ULastEmberAttributeSet_Survival::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Health, OldHealth);
}

void ULastEmberAttributeSet_Survival::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, MaxHealth, OldMaxHealth);
}

void ULastEmberAttributeSet_Survival::OnRep_Hunger(const FGameplayAttributeData& OldHunger)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Hunger, OldHunger);
}

void ULastEmberAttributeSet_Survival::OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, MaxHunger, OldMaxHunger);
}

void ULastEmberAttributeSet_Survival::OnRep_Thirst(const FGameplayAttributeData& OldThirst)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Thirst, OldThirst);
}

void ULastEmberAttributeSet_Survival::OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, MaxThirst, OldMaxThirst);
}

void ULastEmberAttributeSet_Survival::OnRep_Infection(const FGameplayAttributeData& OldInfection)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, Infection, OldInfection);
}

void ULastEmberAttributeSet_Survival::OnRep_MaxInfection(const FGameplayAttributeData& OldMaxInfection)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULastEmberAttributeSet_Survival, MaxInfection, OldMaxInfection);
}
