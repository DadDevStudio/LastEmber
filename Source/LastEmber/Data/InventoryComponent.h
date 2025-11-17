#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryTypes.h"
#include "Engine/DataTable.h"
#include "ItemDefinition.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LASTEMBER_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    // FastArray container (replicated)
    UPROPERTY(Replicated)
    FInventoryArray InventoryItems;

    // DataTable with master definitions (set in editor)
    UPROPERTY(EditDefaultsOnly, Category="Inventory")
    UDataTable* MasterItemTable = nullptr;

    // Max carry weight (server authoritative)
    UPROPERTY(ReplicatedUsing=OnRep_MaxCarryWeight, BlueprintReadOnly)
    float MaxCarryWeight = 100.0f;

    UFUNCTION()
    void OnRep_MaxCarryWeight();

    // RPCs - client -> server requests
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_AddItem(const FName& ItemID, int32 Quantity);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveItem(uint64 ItemInstanceID, int32 TargetIndex);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_UseItem(uint64 ItemInstanceID);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_DropItem(uint64 ItemInstanceID, int32 Quantity, const FVector& DropLocation);

    // Helpers
    UFUNCTION(BlueprintCallable, Category="Inventory")
    float GetTotalWeight() const;

    // Find item index by instance id
    int32 FindItemIndexByInstanceID(uint64 InstanceID) const;

protected:
    virtual void BeginPlay() override;

    // generate new instance id
    uint64 GenerateNewInstanceID();

    // internal add - server side
    bool AddItem_Internal(const FName& ItemID, int32 Quantity);

    // Get definition row (const)
    const FItemDefinitionRow* GetItemDef(const FName& ItemID) const;

    // last assigned
    uint64 LastAssignedInstanceID = 0;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
