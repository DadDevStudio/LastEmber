// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberCharacterBase.cpp
#include "Core/LastEmberCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GAS/LastEmberSurvivalComponent.h"
// #include "GAS/LastEmberInventoryComponent.h"
#include "Directors/BlueprintCameraDirector.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Net/UnrealNetwork.h"
ALastEmberCharacterBase::ALastEmberCharacterBase()
{
	bReplicates = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	SurvivalComponent = CreateDefaultSubobject<ULastEmberSurvivalComponent>("Survival");
	// InventoryComponent = CreateDefaultSubobject<ULastEmberInventoryComponent>("Inventory");
	CameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>("CameraComponent");
	// Utwórz mesh do widoku FP
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(GetRootComponent());
	// FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);      // tylko właściciel widzi FP mesh
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// Domyślnie schowany (bo zaczynamy w TP)
	FirstPersonMesh->SetHiddenInGame(true);
	GetMesh()->SetHiddenInGame(false);
}

void ALastEmberCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		UE_LOG(LogTemp, Log, TEXT("Character initialized on server"));
	// 🔍 Szukamy komponentu lub obiektu typu UBlueprintCameraDirectorEvaluator
	TArray<UActorComponent*> Components;
	GetComponents(Components); // zamiast GetComponentsByClass — działa zawsze
	// for (UActorComponent* Comp : Components)
	// {
	// 	if (Comp->IsA(UBlueprintCameraDirectorEvaluator::StaticClass()))
	// 	{
	// 		CameraDirectorEvaluator = Cast<UBlueprintCameraDirectorEvaluator>(Comp);
	// 	}
	// }
	//
	// if (!CameraDirectorEvaluator)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No CameraDirectorEvaluator found on %s!"), *GetName());
	// }
}

void ALastEmberCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Replikacja niestandardowych zmiennych gdy dodamy survival data
	DOREPLIFETIME(ALastEmberCharacterBase, CamIndex);
	DOREPLIFETIME(ALastEmberCharacterBase, bIsFirstPerson);
}

void ALastEmberCharacterBase::ToggleCameraView()
{
	// Przełącz index i tryb
	CamIndex = (CamIndex == 0) ? 1 : 0;
	bIsFirstPerson = (CamIndex == 1);

	// Przełącz widoczność meshów
	if (bIsFirstPerson)
	{
		// ✅ First Person
		FirstPersonMesh->SetHiddenInGame(false);
		GetMesh()->SetHiddenInGame(true);
	}
	else
	{
		// ✅ Third Person
		FirstPersonMesh->SetHiddenInGame(true);
		GetMesh()->SetHiddenInGame(false);
	}

	// Log do konsoli
	UE_LOG(LogTemp, Log, TEXT("Camera switched: CamIndex = %d | bIsFirstPerson = %s"),
		CamIndex,
		bIsFirstPerson ? TEXT("true") : TEXT("false"));

	// Event do Blueprinta (np. do zmiany kamery GameplayCameraAsset)
	OnCameraModeChanged(bIsFirstPerson);
}
void ALastEmberCharacterBase::OnRep_CameraMode()
{
	// Ta sama logika co w ToggleCameraView(), tylko bez zmiany wartości
	if (bIsFirstPerson)
	{
		FirstPersonMesh->SetHiddenInGame(false);
		GetMesh()->SetHiddenInGame(true);
	}
	else
	{
		FirstPersonMesh->SetHiddenInGame(true);
		GetMesh()->SetHiddenInGame(false);
	}

	OnCameraModeChanged(bIsFirstPerson);
}