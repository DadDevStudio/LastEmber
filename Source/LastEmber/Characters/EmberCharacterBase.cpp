#include "EmberCharacterBase.h"

AEmberCharacterBase::AEmberCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Optionally: set a custom movement component class later
	// GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEmberCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEmberCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
