// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "Core/LastEmberAnimInstanceBase.h"

void ULastEmberAnimInstanceBase::PlaySharedMontage(UAnimMontage* Montage)
{
	if (!Montage)
		return;

	// Thread-safe call — UE5 handles AnimInstance updates on its own anim thread
	Montage_Play(Montage);
}
