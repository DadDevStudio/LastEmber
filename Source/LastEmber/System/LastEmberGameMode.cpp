// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "LastEmberGameMode.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h" // Potrzebne do obsługi czasu

void ALastEmberGameMode::RequestRespawn(AController* Controller, float Delay)
{
	if (Controller)
	{
		// Używamy Timera, żeby wywołać funkcję HandleRespawnDelay za 'Delay' sekund.
		// FTimerDelegate pozwala nam przekazać argument (Controller) do funkcji, która odpali się w przyszłości.
		
		FTimerHandle RespawnTimerHandle;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUObject(this, &ALastEmberGameMode::HandleRespawnDelay, Controller);

		// Ustawiamy timer na serwerze (GameMode istnieje tylko na serwerze)
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, Delay, false);
	}
}

void ALastEmberGameMode::HandleRespawnDelay(AController* Controller)
{
	if (Controller)
	{
		// 1. Jeśli stary pionek (trup) jeszcze istnieje, odpinamy go (Unpossess) i niszczymy (opcjonalnie)
		// RestartPlayer zazwyczaj sam to ogarnia, ale dla pewności:
		APawn* OldPawn = Controller->GetPawn();
		if (OldPawn)
		{
			OldPawn->Destroy(); // Usuwamy trupa ze świata (możesz to usunąć, jeśli trupy mają zostawać)
		}

		// 2. Magiczna funkcja Unreal Engine - znajduje PlayerStart i tworzy nowego Pawna
		RestartPlayer(Controller);
	}
}