// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmberCharacter.h"
#include "AbilitySystemComponent.h"
// Upewnij się, że ta ścieżka pasuje do miejsca, gdzie stworzyłeś plik atrybutów:
#include "Blueprint/UserWidget.h"
#include "LastEmber/GAS/LastEmberAttributeSet_Survival.h" 

ALastEmberCharacter::ALastEmberCharacter()
{
	// Pawn domyślnie może nie mieć włączonego tickowania, włączamy dla pewności
	PrimaryActorTick.bCanEverTick = true;

	// 1. Tworzymy komponent GAS
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	// Ustawienia sieciowe dla GAS
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed - najlepsze dla postaci gracza (Player Character). 
	// Gameplay Effects są liczone na właścicielu (Owner) i serwerze.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 2. Tworzymy Atrybuty (Głód, Pragnienie, Zdrowie)
	SurvivalAttributes = CreateDefaultSubobject<ULastEmberAttributeSet_Survival>(TEXT("SurvivalAttributes"));
}

UAbilitySystemComponent* ALastEmberCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALastEmberCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
    
	// UWAGA: Usuwamy stąd tworzenie HUD! 
	// BeginPlay jest za szybki i często nie ma jeszcze Controllera.
}

void ALastEmberCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
    
	// Klient właśnie dostał kontroler -> Tworzymy HUD
	InitHUD();
}

void ALastEmberCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Serwer/Host przejął postać -> Inicjujemy GAS i HUD
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// Tworzymy HUD (tylko dla Hosta, bo dedykowany serwer nie ma ekranu)
	InitHUD();
}

void ALastEmberCharacter::InitHUD()
{
	// Zabezpieczenie: Twórz tylko jeśli to lokalny gracz i nie mamy jeszcze HUDu
	if (IsLocallyControlled() && HUDWidgetClass && !HUDInstance)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			HUDInstance = CreateWidget<ULastEmberHUDWidget>(PC, HUDWidgetClass);
			if (HUDInstance)
			{
				HUDInstance->AddToViewport();
				HUDInstance->InitializeWithGAS(AbilitySystemComponent, SurvivalAttributes);
			}
		}
	}
}