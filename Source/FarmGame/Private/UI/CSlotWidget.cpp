#include "UI/CSlotWidget.h"
#include "UI/CInventoryWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"
#include "UI/CSlotDropDownWidget.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CGameInstance.h"
#include "Interfaces/CItemInterface.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UCSlotWidget::NativeOnInitialized()
{
}

void UCSlotWidget::SetItem(const FInventorySlot& SlotData)
{
	CurrentSlotData = MakeShared<FInventorySlot>(SlotData);
	CurrentItemID = SlotData.ItemID;

	if (CurrentItemID == EItemID::None)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
		ItemCountText->SetVisibility(ESlateVisibility::Hidden);
		ItemDurabilityBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Visible);
		ItemCountText->SetVisibility(ESlateVisibility::Visible);
		if (SlotData.ItemIcon)
		{
			ItemIconImage->SetBrushFromTexture(SlotData.ItemIcon);
		}
		else
		{
			UE_LOG(LogItem, Error, TEXT("ItemIconTexture is missing! ItemID : %s"), *(UEnum::GetValueAsString(CurrentItemID)));
		}
		ItemCountText->SetText(FText::AsNumber(SlotData.CurrentStack));
		if (GetSlotItemData()->CurrentDurability>0.0f)
		{
			ItemDurabilityBar->SetVisibility(ESlateVisibility::Visible);
			ItemDurabilityBar->SetPercent(GetSlotItemData()->CurrentDurability / GetSlotItemData()->MaxDurability);
		}
	}
}

FInventorySlot* UCSlotWidget::GetSlotItemData()
{
	return CurrentSlotData.Get();
}

FReply UCSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return FReply::Unhandled();
}

void UCSlotWidget::NativeOnDragDetected(const FGeometry& InGemoetry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (CurrentItemID == EItemID::None) return;

	UCInventorySlotDragDropOperation* DragOperation = NewObject<UCInventorySlotDragDropOperation>();
	DragOperation->SourceSlot = this;

	OutOperation = DragOperation;
}



void UCSlotWidget::SetParentWidget(UUserWidget* InParent)
{
	return;
}
