// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


// LastEmberGameState.cpp
#include "Core/LastEmberGameState.h"
#include "Net/UnrealNetwork.h"

ALastEmberGameState::ALastEmberGameState()
{
	TimeOfDay = 12.0f;
}

void ALastEmberGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALastEmberGameState, TimeOfDay);
	DOREPLIFETIME(ALastEmberGameState, CurrentWeather);
}

void ALastEmberGameState::OnRep_TimeOfDay()
{
	// Klient może np. aktualizować światło lub UI czasu
	UE_LOG(LogTemp, Log, TEXT("Client: Time updated to %.2f"), TimeOfDay);
}

void ALastEmberGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		TimeOfDay += DeltaTime * 0.1f;
		if (TimeOfDay >= 24.0f)
			TimeOfDay = 0.0f;
	}
}
