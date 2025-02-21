#include "UI/CSlotWidget.h"
#include "UI/CInventoryWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"
#include "UI/CSlotDropDownWidget.h"
#include "Global.h"
#include "Components/CInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CGameInstance.h"
#include "Interfaces/CItemInterface.h"

void UCSlotWidget::NativeOnInitialized()
{
}

void UCSlotWidget::SetItem(const FInventorySlot& SlotData)
{
	CurrentSlotData = SlotData;

	if (CurrentSlotData.ItemID == EItemID::None)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
		ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Visible);
		ItemCountText->SetVisibility(ESlateVisibility::Visible);
		if (CurrentSlotData.ItemIcon)
		{
			ItemIconImage->SetBrushFromTexture(CurrentSlotData.ItemIcon);
		}
		else
		{
			UE_LOG(LogItem, Error, TEXT("ItemIconTexture is missing! ItemID : %s"), *(UEnum::GetValueAsString(CurrentSlotData.ItemID)));
		}
		ItemCountText->SetText(FText::AsNumber(CurrentSlotData.CurrentStack));
	}
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
	if (CurrentSlotData.ItemID == EItemID::None) return;

	UCInventorySlotDragDropOperation* DragOperation = NewObject<UCInventorySlotDragDropOperation>();
	DragOperation->DraggedSlotData = MakeShareable(&InventoryComponent->InventorySlots[SlotIndex]);
	DragOperation->SourceSlot = this;

	OutOperation = DragOperation;
}

bool UCSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UCInventorySlotDragDropOperation* DragOperation = Cast<UCInventorySlotDragDropOperation>(InOperation))
	{
		if (DragOperation->SourceSlot != this)
		{
			InventoryComponent->SwapSlot(DragOperation->SourceSlot->SlotIndex, SlotIndex);
			return true;
		}
	}
	return false;
}

void UCSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentSlotData.ItemID != EItemID::None)
	{
		FVector2D MousePoseition = InMouseEvent.GetScreenSpacePosition();
		ParentInventoryWidget->ShowExplainWidget(CurrentSlotData, MousePoseition);
	}
}

void UCSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	ParentInventoryWidget->HideExplainWidget();
}

FReply UCSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OpenDropDownMenu();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UCSlotWidget::OpenDropDownMenu()
{
	if (!SlotDropDownWidget) return;

	SlotDropDownWidget->SetItem(CurrentSlotData.ItemID);
	SlotDropDownWidget->SetVisibility(ESlateVisibility::Visible);
}
