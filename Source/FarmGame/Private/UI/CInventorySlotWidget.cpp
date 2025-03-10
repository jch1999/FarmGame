#include "UI/CInventorySlotWidget.h"
#include "UI/CInventoryWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"
#include "UI/CSlotDropDownWidget.h"
#include "Global.h"
#include "CGameInstance.h"
#include "Interfaces/CItemInterface.h"
#include "Blueprint/WidgetLayoutLibrary.h"

bool UCInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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
				InventoryComp->SwapSlot(DragOperation->SourceSlot->SlotIndex, SlotIndex);
			}
			return true;
		}
	}
	return false;
}

void UCInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentItemID != EItemID::None && ParentInventoryWidget)
	{
		FVector2D MousePoseition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		ParentInventoryWidget->ShowExplainWidget(CurrentSlotData, MousePoseition);
	}
}

void UCInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	GetWorld()->GetTimerManager().SetTimer(HideExplainTimer, this, &UCInventorySlotWidget::HideEplainWidget, 0.1f, false);
}

FReply UCInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OpenDropDownMenu();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UCInventorySlotWidget::OpenDropDownMenu()
{
	if (!SlotDropDownWidget) return;

	SlotDropDownWidget->SetItem(CurrentSlotData);
	SlotDropDownWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCInventorySlotWidget::SetParentWidget(UUserWidget* InParent)
{
	if (UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(InParent))
	{
		ParentInventoryWidget = InventoryWidget;
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("InventorySlotWidget Error! Cast Failed UUser* InParent -> InventoryWidget*"));
	}
}

void UCInventorySlotWidget::HideEplainWidget()
{
	if (ParentInventoryWidget && !ParentInventoryWidget->IsInExpainWidget())
	{
		ParentInventoryWidget->HideExplainWidget();
	}
}
