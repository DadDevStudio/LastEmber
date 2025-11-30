// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

// LastEmberCharacterBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/LastEmberAnimSyncComponent.h"
#include "LastEmberCharacterBase.generated.h"

class ULastEmberSurvivalComponent;
class ULastEmberInventoryComponent;
class UAbilitySystemComponent;
class UGameplayCameraComponent;

UCLASS()
class LASTEMBER_API ALastEmberCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALastEmberCharacterBase();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ToggleCameraView();
	
	// Event do obsługi w BP — np. do zmiany kamery lub mesha
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void OnCameraModeChanged(bool bFirstPerson);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	TObjectPtr<ULastEmberAnimSyncComponent> AnimSyncComponent;

	// Mesh używany w trybie pierwszoosobowym
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* FirstPersonMesh;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULastEmberSurvivalComponent* SurvivalComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGameplayCameraComponent* CameraComponent;
	/** Kamera BlueprintCameraDirectorEvaluator (przechowywana jako UObject) */
	UPROPERTY()
	UObject* CameraDirectorEvaluator;
	
	// Index trybu kamery: 0 = ThirdPerson, 1 = FirstPerson
	UPROPERTY(ReplicatedUsing=OnRep_CameraMode, EditAnywhere, BlueprintReadWrite, Category="Camera")
	int32 CamIndex = 0;

	// Czy aktualnie w trybie FirstPerson
	UPROPERTY(ReplicatedUsing=OnRep_CameraMode, EditAnywhere, BlueprintReadWrite, Category="Camera")
	bool bIsFirstPerson = false;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// ULastEmberInventoryComponent* InventoryComponent;
	UFUNCTION()
	void OnRep_CameraMode();
	void PlayReload();
};
