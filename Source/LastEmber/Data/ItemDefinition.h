#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Consumable,
	IT_Equipment,
	IT_Backpack,
	IT_Weapon,
	IT_Material,
	IT_Ammo,
	IT_Other
};

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	ES_None,
	ES_Head,
	ES_Chest,
	ES_Back,
	ES_Hands,
	ES_Feet,
	ES_Primary,
	ES_Secondary
};

USTRUCT(BlueprintType)
struct FItemDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

	// Key (FName) will be the row name in the DataTable ("HealthPotion", "UrbanBackpack" etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::IT_Other;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipSlot EquipSlot = EEquipSlot::ES_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.0f; // weight per single unit

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
    
	// Optional: skeletal/static mesh for preview
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> PreviewStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> PreviewSkeletalMesh;
};
