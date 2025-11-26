// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LEGameMode.generated.h"

class ALEPlayerController;
/**
 * 
 */
UCLASS()
class LASTEMBER_API ALEGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Constuctor
	ALEGameMode();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
