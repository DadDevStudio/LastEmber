#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 ItemInstanceID = 0;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	int32 Quantity = 1;

	UPROPERTY()
	float Durability = 100.f;

	// helper: index of owning array entry (not necessary, included for extensibility)
	UPROPERTY()
	uint8 OwnerIndex = 0;
};

USTRUCT()
struct FInventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItem> Items;

	// Owning component (not replicated)
	UPROPERTY(NotReplicated)
	class UInventoryComponent* Owner;

	// Mark dirty by reference to item
	void MarkItemDirty(FInventoryItem& Item)
	{
		FFastArraySerializer::MarkItemDirty(Item);
	}
};