#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/EmberCharacterBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// ROOT
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// MESH
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);

	SetReplicates(true);
}

void AWeaponBase::OnEquipped(AEmberCharacterBase* Character)
{
	if (!Character) return;

	AttachToComponent(
		Character->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		AttachSocket
	);
}

void AWeaponBase::OnUnequipped()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
