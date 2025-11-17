#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class LASTEMBER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:

	/** Główny Root — MUSI istnieć */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	/** Mesh broni */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* WeaponMesh;

public:

	/** Nazwa socketa w szkielecie postaci */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName AttachSocket = TEXT("WeaponSocket");

	/** Wywoływane kiedy postać wyciąga broń */
	virtual void OnEquipped(class AEmberCharacterBase* Character);

	/** Wywoływane kiedy postać odkłada broń */
	virtual void OnUnequipped();

	/** Zwraca mesh broni */
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
