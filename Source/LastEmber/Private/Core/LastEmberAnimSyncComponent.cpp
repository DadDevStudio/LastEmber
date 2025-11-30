// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "Core/LastEmberAnimSyncComponent.h"
#include "Core/LastEmberAnimInstanceBase.h"
#include "Core/LastEmberCharacterBase.h"

ULastEmberAnimSyncComponent::ULastEmberAnimSyncComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULastEmberAnimSyncComponent::BeginPlay()
{
	Super::BeginPlay();

	ALastEmberCharacterBase* Owner = Cast<ALastEmberCharacterBase>(GetOwner());
	if (Owner)
	{
		FP_Mesh = Owner->FirstPersonMesh;
		TP_Mesh = Owner->GetMesh();
	}
}

void ULastEmberAnimSyncComponent::PlaySharedMontage(UAnimMontage* Montage)
{
	if (!Montage)
		return;

	if (FP_Mesh && FP_Mesh->GetAnimInstance())
	{
		if (ULastEmberAnimInstanceBase* FPAnim = Cast<ULastEmberAnimInstanceBase>(FP_Mesh->GetAnimInstance()))
		{
			FPAnim->PlaySharedMontage(Montage);
		}
	}

	if (TP_Mesh && TP_Mesh->GetAnimInstance())
	{
		if (ULastEmberAnimInstanceBase* TPAnim = Cast<ULastEmberAnimInstanceBase>(TP_Mesh->GetAnimInstance()))
		{
			TPAnim->PlaySharedMontage(Montage);
		}
	}
}
