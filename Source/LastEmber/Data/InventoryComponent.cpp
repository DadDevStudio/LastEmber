#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
    SetIsReplicatedByDefault(true);
    InventoryItems.Owner = this;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    // If needed, initialize MaxCarryWeight from owner/backpack
}

void UInventoryComponent::OnRep_MaxCarryWeight()
{
    // Notify UI or owner about change
}

uint64 UInventoryComponent::GenerateNewInstanceID()
{
    return ++LastAssignedInstanceID;
}

const FItemDefinitionRow* UInventoryComponent::GetItemDef(const FName& ItemID) const
{
    if (!MasterItemTable) return nullptr;
    static const FString ContextString(TEXT("ItemDefLookup"));
    return MasterItemTable->FindRow<FItemDefinitionRow>(ItemID, ContextString);
}

bool UInventoryComponent::AddItem_Internal(const FName& ItemID, int32 Quantity)
{
    if (Quantity <= 0) return false;
    const FItemDefinitionRow* Def = GetItemDef(ItemID);
    if (!Def) return false;

    // Compute additional weight
    const float AddedWeight = Def->Weight * Quantity;

    if (GetTotalWeight() + AddedWeight > MaxCarryWeight)
    {
        // Not allowed - overweight
        return false;
    }

    // If stackable, try merge
    if (Def->MaxStack > 1)
    {
        for (FInventoryItem& Entry : InventoryItems.Items)
        {
            if (Entry.ItemID == ItemID && Entry.Quantity < Def->MaxStack)
            {
                const int32 Space = Def->MaxStack - Entry.Quantity;
                const int32 ToAdd = FMath::Min(Space, Quantity);
                Entry.Quantity += ToAdd;
                Quantity -= ToAdd;
                InventoryItems.MarkItemDirty(Entry);

                if (Quantity <= 0)
                {
                    InventoryItems.MarkArrayDirty();
                    return true;
                }
            }
        }
    }

    // If any quantity remains -> create new entries (respect MaxStack)
    while (Quantity > 0)
    {
        int32 CreateCount = Quantity;
        if (Def->MaxStack > 1 && CreateCount > Def->MaxStack)
            CreateCount = Def->MaxStack;

        FInventoryItem NewEntry;
        NewEntry.ItemInstanceID = GenerateNewInstanceID();
        NewEntry.ItemID = ItemID;
        NewEntry.Quantity = CreateCount;
        InventoryItems.Items.Add(NewEntry);
        Quantity -= CreateCount;
    }

    InventoryItems.MarkArrayDirty();
    return true;
}

// RPC Implementations and validations
void UInventoryComponent::Server_AddItem_Implementation(const FName& ItemID, int32 Quantity)
{
    // Server-side validation: sanity checks, anti-cheat, item existence etc.
    AddItem_Internal(ItemID, Quantity);
}
bool UInventoryComponent::Server_AddItem_Validate(const FName& ItemID, int32 Quantity)
{
    return !ItemID.IsNone() && Quantity > 0 && Quantity < 1000000;
}

void UInventoryComponent::Server_MoveItem_Implementation(uint64 ItemInstanceID, int32 TargetIndex)
{
    int32 SrcIdx = FindItemIndexByInstanceID(ItemInstanceID);
    if (SrcIdx == INDEX_NONE) return;

    if (TargetIndex < 0) TargetIndex = 0;
    if (TargetIndex >= InventoryItems.Items.Num()) TargetIndex = InventoryItems.Items.Num() - 1;

    FInventoryItem ItemCopy = InventoryItems.Items[SrcIdx];
    InventoryItems.Items.RemoveAt(SrcIdx);
    InventoryItems.Items.Insert(ItemCopy, TargetIndex);
    InventoryItems.MarkArrayDirty();
}
bool UInventoryComponent::Server_MoveItem_Validate(uint64 ItemInstanceID, int32 TargetIndex) { return true; }

void UInventoryComponent::Server_UseItem_Implementation(uint64 ItemInstanceID)
{
    int32 Idx = FindItemIndexByInstanceID(ItemInstanceID);
    if (Idx == INDEX_NONE) return;

    FInventoryItem& Entry = InventoryItems.Items[Idx];
    const FItemDefinitionRow* Def = GetItemDef(Entry.ItemID);
    if (!Def) return;

    // Example: consumable decreases quantity and maybe apply effects via GAS (not here)
    Entry.Quantity -= 1;
    if (Entry.Quantity <= 0)
        InventoryItems.Items.RemoveAt(Idx);
    else
        InventoryItems.MarkItemDirty(Entry);

    InventoryItems.MarkArrayDirty();
}
bool UInventoryComponent::Server_UseItem_Validate(uint64 ItemInstanceID) { return true; }

void UInventoryComponent::Server_DropItem_Implementation(uint64 ItemInstanceID, int32 Quantity, const FVector& DropLocation)
{
    int32 Idx = FindItemIndexByInstanceID(ItemInstanceID);
    if (Idx == INDEX_NONE) return;

    FInventoryItem& Entry = InventoryItems.Items[Idx];
    if (Quantity <= 0 || Quantity > Entry.Quantity) Quantity = Entry.Quantity;

    Entry.Quantity -= Quantity;
    if (Entry.Quantity <= 0) InventoryItems.Items.RemoveAt(Idx);
    else InventoryItems.MarkItemDirty(Entry);

    InventoryItems.MarkArrayDirty();

    // Spawn pickup actor on server - omitted (create BP InventoryActor spawn with ItemID, Quantity)
}
bool UInventoryComponent::Server_DropItem_Validate(uint64 ItemInstanceID, int32 Quantity, const FVector& DropLocation) { return true; }

float UInventoryComponent::GetTotalWeight() const
{
    float Total = 0.f;
    for (const FInventoryItem& Entry : InventoryItems.Items)
    {
        const FItemDefinitionRow* Def = GetItemDef(Entry.ItemID);
        if (Def)
            Total += Def->Weight * Entry.Quantity;
        else
            Total += 0.f; // unknown items weigh 0
    }
    return Total;
}

int32 UInventoryComponent::FindItemIndexByInstanceID(uint64 InstanceID) const
{
    for (int32 i = 0; i < InventoryItems.Items.Num(); ++i)
    {
        if (InventoryItems.Items[i].ItemInstanceID == InstanceID) return i;
    }
    return INDEX_NONE;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, InventoryItems);
    DOREPLIFETIME(UInventoryComponent, MaxCarryWeight);
}
