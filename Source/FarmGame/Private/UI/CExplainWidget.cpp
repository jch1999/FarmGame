#include "UI/CExplainWidget.h"
#include "Interfaces/CItemInterface.h"
#include "Components/CInventoryComponent.h"
#include "CGameInstance.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCExplainWidget::SetItem(TWeakPtr<FInventorySlot> InSlotData)
{
    if (!InSlotData.IsValid()) return; 
    auto SlotData = InSlotData.Pin();
    ItemIconImage->SetBrushFromTexture(SlotData->ItemIcon);

    FString ItemFullName = UEnum::GetValueAsString(SlotData->ItemID);
    ItemFullName.RemoveFromStart("EItemID::");
    FString ItemName, ItemQuality;
    if (ItemFullName.Split("_", &ItemName, &ItemQuality))
    {
        ItemNameText->SetText(FText::FromString(ItemName));
        ItemExtraText->SetText(FText::FromString(ItemQuality));
    }
    else
    {
        ItemNameText->SetText(FText::FromString(ItemFullName));
        ItemExtraText->SetText(FText::FromString(""));
    }
    ItemDescriptionText->SetText(FText::FromString(SlotData->Description));
}
