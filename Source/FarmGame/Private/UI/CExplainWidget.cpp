#include "UI/CExplainWidget.h"
#include "Interfaces/CItemInterface.h"
#include "Components/CInventoryComponent.h"
#include "CGameInstance.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

void UCExplainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    bIsMouseOver = false; 
    SetVisibility(ESlateVisibility::Visible);
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
            if (!IsHovered())
            {
                this->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 2.0f, false);
}

FReply UCExplainWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Unhandled();
}

FReply UCExplainWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Unhandled();
}

FReply UCExplainWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Unhandled();
}

FReply UCExplainWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (ItemDescriptionScrollBox)
    {
        float CurrentOffset = ItemDescriptionScrollBox->GetScrollOffset();

        // 마우스 휠 방향에 따라 이동량 계산
        float ScrollAmount = -InMouseEvent.GetWheelDelta() * 50.0f; // 50은 스크롤 속도 조정

        float NewOffset = FMath::Clamp(CurrentOffset + ScrollAmount, 0.0f, ItemDescriptionScrollBox->GetScrollOffsetOfEnd());

        ItemDescriptionScrollBox->SetScrollOffset(NewOffset);
        return FReply::Handled(); // 내가 처리했으니 부모로 전달 X
    }

    return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
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

    UCGameInstance* MyGI = Cast<UCGameInstance>(GetGameInstance());
    if (const TOptional<FItemData>& ItemDataOpt = MyGI->GetItemtData(SlotData->ItemID))
    {
        if (ItemDataOpt.IsSet())
        {
            FItemData ItemData = ItemDataOpt.GetValue();
            ItemPriceText->SetText(FText::FromString(FString::FromInt(ItemData.SellPrice)));
        }
    }
}

void UCExplainWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
}
