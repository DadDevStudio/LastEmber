// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmberCharacter.h"
#include "AbilitySystemComponent.h"
#include "LastEmber/GAS/LastEmberAttributeSet_Survival.h"
#include "LastEmber/UI/LastEmberHUDWidget.h" // Twój Widget
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" // Do debugowania linii (DrawDebugLine)
#include "LastEmber/System/LastEmberGameMode.h"

// --- WAŻNE NAGŁÓWKI DO NAPRAWY BŁĘDÓW ---
#include "MoverComponent.h"                // Naprawia błąd "UMoverComponent undefined"
#include "Components/CapsuleComponent.h"   // Naprawia błąd kapsuły
#include "Components/SkeletalMeshComponent.h" // Naprawia błąd mesha

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
void ALastEmberCharacter::HandleDeath()
{
	// [SERWER]
	if (bIsDead) return;
	bIsDead = true;

	if (Controller)
	{
		DisableInput(Cast<APlayerController>(Controller));
	}

	Multicast_Die();

	// Respawn Logic
	ALastEmberGameMode* GM = Cast<ALastEmberGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && Controller)
	{
		GM->RequestRespawn(Controller, 5.0f);
	}
}

void ALastEmberCharacter::Multicast_Die_Implementation()
{
	// --- TUTAJ BYŁY BŁĘDY ---
	
	// 1. Mover Component (Szukamy go dynamicznie)
	// Używamy GetComponentByClass zamiast FindComponentByClass (nowocześniejsze API Actora)
	UMoverComponent* MoverComp = GetComponentByClass<UMoverComponent>();
	if (MoverComp)
	{
		MoverComp->SetComponentTickEnabled(false);
		MoverComp->Deactivate();
	}

	// 2. Capsule Component
	// Ponieważ dziedziczymy po APawn, nie mamy funkcji GetCapsuleComponent().
	// Musimy znaleźć komponent ręcznie.
	UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 3. Mesh Component (Ragdoll)
	// Tak samo - APawn nie ma funkcji GetMesh(). Szukamy SkeletalMeshComponent.
	USkeletalMeshComponent* MeshComp = GetComponentByClass<USkeletalMeshComponent>();
	if (MeshComp)
	{
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		// --- NOWOŚĆ: ODPIĘCIE OD KAPSUŁY ---
		// Mówimy: "Zostań w tym miejscu świata, gdzie jesteś, ale nie bądź już dzieckiem Kapsuły"
		MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MeshComp->SetSimulatePhysics(true);
	}

	// 4. UI
	if (IsLocallyControlled() && HUDInstance)
	{
		HUDInstance->RemoveFromParent();
	}
}
// 2. Główna funkcja interakcji
void ALastEmberCharacter::OnInteractInput()
{
	// 1. [KLIENT] Raycast i namierzanie
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 2000.0f); // 20 metrów

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	// Debug wizualny (tylko dla Ciebie lokalnie)
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 2.0f);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<ULastEmberInteractable>())
		{
			// ZAMIAST OD RAZU ROBIĆ INTERAKCJĘ...
			// ILastEmberInteractable::Execute_Interact(HitActor, this); <-- TO BYŁO LOKALNE (ZŁE)
			
			// ...WOŁAMY SERWER:
			Server_Interact(HitActor);
		}
	}
}

// Implementacja funkcji serwerowej (musi mieć dopisek _Implementation)
void ALastEmberCharacter::Server_Interact_Implementation(AActor* TargetActor)
{
	// 2. [SERWER] To wykonuje się na serwerze!
	// Tutaj moglibyśmy dodać sprawdzenie odległości (anty-cheat), ale na razie ufamy klientowi.

	if (TargetActor && TargetActor->Implements<ULastEmberInteractable>())
	{
		// Serwer wykonuje interakcję.
		// Dzięki temu "Instigator Pawn" w Blueprincie puszki będzie Serwerową wersją postaci (tą, która ma władzę nad GAS).
		ILastEmberInteractable::Execute_Interact(TargetActor, this);
	}
}