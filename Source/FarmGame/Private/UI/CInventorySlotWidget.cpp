#include "UI/CInventorySlotWidget.h"
#include "UI/CInventoryWidget.h"
#include "UI/CSlotDragDropOperation.h"
#include "UI/CSlotDropDownWidget.h"
#include "Global.h"
#include "CGameInstance.h"
#include "Interfaces/Item.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/CSlotWidget.h"
#include "Components/Image.h"
#include "CGameInstance.h"

bool UCInventorySlotWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SlotType = ESlotType::Inventory_Player;
	return true;
}

bool UCInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool bHandled = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (UCSlotDragDropOperation* DragOperation = Cast<UCSlotDragDropOperation>(InOperation))
	{
		if (!DragOperation->SourceSlot)
		{
			UE_LOG(LogItem, Error, TEXT("SourceSlot is nullptr in UCSlotWidget::NativeOnDrop"));
			return bHandled;
		}
		
		if (DragOperation->SourceSlot != this)
		{
			if (UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(ParentWidget))
			{
				if (UCInventoryComponent* InventoryComp = InventoryWidget->InventoryComp)
				{
					switch (DragOperation->SourceSlot->GetSlotType())
					{
					case ESlotType::Inventory_Player:
					{
						UE_LOG(LogItem, Log, TEXT("SwapSlot called with SlotIndex1: %d, SlotIndex2: %d"), DragOperation->SourceSlot->GetSlotIndex(), SlotIndex);
						InventoryComp->SwapSlot(DragOperation->SourceSlot->GetSlotIndex(), GetSlotIndex());
					}
					break;
					default:
						break;
					}
				}
			}
			return bHandled;
		}
	}
	return bHandled;
	
}

void UCInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(ParentWidget);
	if (CurrentItemID != EItemID::None && InventoryWidget)
	{
		FVector2D MousePoseition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		InventoryWidget->ShowExplainWidget(CurrentSlotData, MousePoseition);
	}
}

void UCInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (GetWorld()->GetTimerManager().TimerExists(HideExplainTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(HideExplainTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(HideExplainTimer, this, &UCInventorySlotWidget::HideEplainWidget, 0.1f, false);
}

FReply UCInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OpenDropDownMenu();
		return FReply::Handled();
	}
	return Reply;
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
		ParentWidget = InventoryWidget;
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("InventorySlotWidget Error! Cast Failed UUser* InParent -> InventoryWidget*"));
	}
}

void UCInventorySlotWidget::HideEplainWidget()
{
	UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(ParentWidget);
	if (InventoryWidget && !InventoryWidget->IsInExpainWidget())
	{
		InventoryWidget->HideExplainWidget();
	}
}
