// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h" // Ważne: Dziedziczymy po Pawn, nie Character
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "LastEmber/UI/LastEmberHUDWidget.h"
#include "LastEmber/Interfaces/LastEmberInteractable.h"
#include "LastEmberCharacter.generated.h"

// Forward declaration - mówimy, że te klasy istnieją, żeby nie zaśmiecać nagłówka
class UAbilitySystemComponent;
class ULastEmberAttributeSet_Survival;

UCLASS()
class LASTEMBER_API ALastEmberCharacter : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Konstruktor
	ALastEmberCharacter();

	// --- GAS (Gameplay Ability System) ---
	
	// Komponent zarządzający umiejętnościami (serce systemu)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LastEmber|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Zestaw atrybutów: Zdrowie, Głód, Pragnienie
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LastEmber|GAS")
	TObjectPtr<ULastEmberAttributeSet_Survival> SurvivalAttributes;

	// Wymagane przez interfejs IAbilitySystemInterface.
	// Dzięki temu inne systemy wiedzą, że ta postać ma statystyki.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// To woła AttributeSet, gdy HP spadnie do 0
	virtual void HandleDeath();
protected:
	virtual void BeginPlay() override;
	
	// W sekcji public lub protected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LastEmber|UI")
	TSubclassOf<class ULastEmberHUDWidget> HUDWidgetClass; // Klasa widgetu do stworzenia

	UPROPERTY()
	TObjectPtr<class ULastEmberHUDWidget> HUDInstance; // Instancja (żywy obiekt)
	// --- NOWE FUNKCJE DO OBSŁUGI HUD ---
    
	// Wywoływane na Kliencie, gdy serwer prześle informację "Hej, to jest twój kontroler"
	virtual void OnRep_Controller() override;

	// Wywoływane na Serwerze (i Hoście), gdy kontroler przejmuje postać
	virtual void PossessedBy(AController* NewController) override;

	// Nasza prywatna funkcja, która faktycznie tworzy widget
	void InitHUD();

	// ... zmienne HUDWidgetClass i HUDInstance ...
	

	// Zmienna, żeby nie umrzeć dwa razy w jednej klatce
	bool bIsDead = false;

	// Funkcja sieciowa typu Multicast - wywołana na serwerze, wykonuje się u wszystkich
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Die();
	// Funkcja podpięta pod klawisz E
	// Zmieniamy to na UFUNCTION, żeby Blueprint to widział
	UFUNCTION(BlueprintCallable, Category = "LastEmber|Interaction")
	void OnInteractInput();

	// Zmienna do trzymania Input Action (przypiszemy w Blueprincie)
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IA_Interact;
	
	// Nowa funkcja: Server RPC. 
	// "Reliable" oznacza, że pakiet musi dotrzeć (ważne dla rozgrywki).
	// "WithValidation" to zabezpieczenie (np. czy gracz nie oszukuje), Unreal wymaga tego w starszych wersjach, 
	// ale w nowszych wystarczy samo Reliable. Zrobimy wersję prostą.
	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* TargetActor);
};