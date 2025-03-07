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

bool UCSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	if (UCInventorySlotDragDropOperation* DragOperation = Cast<UCInventorySlotDragDropOperation>(InOperation))
	{
		if (!DragOperation->SourceSlot)
		{
			UE_LOG(LogItem, Error, TEXT("SourceSlot is nullptr in UCSlotWidget::NativeOnDrop"));
			return false;
		}
		if (DragOperation->SourceSlot != this)
		{
			if (UCInventoryComponent* InventoryComp = ParentInventoryWidget->InventoryComp)
			{
				UE_LOG(LogItem, Log, TEXT("SwapSlot called with SlotIndex1: %d, SlotIndex2: %d"), DragOperation->SourceSlot->SlotIndex, SlotIndex);
				InventoryComponent->SwapSlot(DragOperation->SourceSlot->SlotIndex, SlotIndex);
			}
			return true;
		}
	}
	return false;
}

void UCSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentItemID != EItemID::None && ParentInventoryWidget)
	{
		FVector2D MousePoseition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		ParentInventoryWidget->ShowExplainWidget(CurrentSlotData, MousePoseition);
	}
}

void UCSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	GetWorld()->GetTimerManager().SetTimer(HideExplainTimer, this, &UCSlotWidget::HideEplainWidget, 0.1f, false);
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

	SlotDropDownWidget->SetItem(CurrentSlotData);
	SlotDropDownWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCSlotWidget::HideEplainWidget()
{
	if (ParentInventoryWidget && !ParentInventoryWidget->IsInExpainWidget())
	{
		ParentInventoryWidget->HideExplainWidget();
	}
}
