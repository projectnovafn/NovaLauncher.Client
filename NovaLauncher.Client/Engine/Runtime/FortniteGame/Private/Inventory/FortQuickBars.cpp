#include "Inventory/FortQuickBars.h"
#include "Player/FortPlayerController.h"

#include "Nova.h"

void AFortQuickBars::AddItemInternal(
    FGuid ItemGuid,
    EFortQuickBars QuickBarType,
    int& OutSlotIndex,
    AddItemInternalOriginal Callback)
{
    auto PlayerController = Cast<AFortPlayerController>(GetOwner());
    if (!PlayerController)
        return;

    auto Item = PlayerController->GetInventoryItemWithGuid(ItemGuid);
    if (Item == nullptr)
        return;

    auto ItemDefinition = Item->ItemEntry.ItemDefinition;
    auto ItemType = ItemDefinition->GetWeaponType();
    int PreferredSlot = GNova->GetPreferredSlot(ItemType);

    if (PreferredSlot == -1)
        return;

    auto& Slot = PrimaryQuickBar.Slots[PreferredSlot];

    if (!Slot.Items.IsValidIndex(0))
    {
        OutSlotIndex = PreferredSlot;
        return;
    }

    FGuid CurrentItemGuid = Slot.Items[0];
    auto CurrentItem = PlayerController->GetInventoryItemWithGuid(CurrentItemGuid);
    if (CurrentItem == nullptr)
        return;

    auto CurrentItemDefinition = CurrentItem->ItemEntry.ItemDefinition;
    auto CurrentItemType = CurrentItemDefinition->GetWeaponType();

    if (ItemType == CurrentItemType && CurrentItemDefinition->Rarity < ItemDefinition->Rarity)
    {
        int NextSlot = PrimaryQuickBar.GetNextSlot();
        if (NextSlot != -1)
        {
            OutSlotIndex = PreferredSlot;
            Slot.Items.Empty();

            Callback(this, CurrentItemGuid, QuickBarType, NextSlot);
        }
    }
}