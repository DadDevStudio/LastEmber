// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LastEmberAnimSyncComponent.generated.h"

class ULastEmberAnimInstanceBase;
class USkeletalMeshComponent;

/**
 * Handles synchronized animation events between FP and TP meshes.
 * Safe for multiplayer and threaded animation update.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LASTEMBER_API ULastEmberAnimSyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULastEmberAnimSyncComponent();

	UFUNCTION(BlueprintCallable, Category="Animation")
	void PlaySharedMontage(UAnimMontage* Montage);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USkeletalMeshComponent* FP_Mesh;

	UPROPERTY()
	USkeletalMeshComponent* TP_Mesh;
};
