#include "UI/CExplainWidget.h"
#include "Interfaces/CItemInterface.h"
#include "Components/CInventoryComponent.h"
#include "CGameInstance.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCExplainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    bIsMouseOver = false;
}

void UCExplainWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
    bIsMouseOver = true;
    GetWorld()->GetTimerManager().ClearTimer(HideTimer);
}

void UCExplainWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    bIsMouseOver = false;

    GetWorld()->GetTimerManager().ClearTimer(HideTimer);
    GetWorld()->GetTimerManager().SetTimer(HideTimer, 
        [this]() {
            this->SetVisibility(ESlateVisibility::Hidden);
        }, 2.0f, false);
}

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

void UCExplainWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
}
