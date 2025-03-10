#include "UI/CInventorySlotWidget.h"
#include "UI/CInventoryWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"
#include "UI/CSlotDropDownWidget.h"
#include "Global.h"
#include "CGameInstance.h"
#include "Interfaces/CItemInterface.h"
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

	if (UCInventorySlotDragDropOperation* DragOperation = Cast<UCInventorySlotDragDropOperation>(InOperation))
	{
		if (!DragOperation->SourceSlot)
		{
			UE_LOG(LogItem, Error, TEXT("SourceSlot is nullptr in UCSlotWidget::NativeOnDrop"));
			return false;
		}
		if (DragOperation->SourceSlot != this)
		{
			UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(ParentWidget);
			if (UCInventoryComponent* InventoryComp = InventoryWidget->InventoryComp)
			{
				switch (DragOperation->SourceSlot->SlotType)
				{
				case ESlotType::Inventory_Player:
				{
					UE_LOG(LogItem, Log, TEXT("SwapSlot called with SlotIndex1: %d, SlotIndex2: %d"), DragOperation->SourceSlot->SlotIndex, SlotIndex);
					InventoryComp->SwapSlot(DragOperation->SourceSlot->SlotIndex, SlotIndex);
				}
					break;
				default:
					break;
				}
				DragOperation->SourceSlot->ItemIconImage->SetOpacity(1.0f);

				if (UGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>())
				{
					if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
					{
						MyGI->StopDragging();
					}
				}
			}
			return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);;
		}
	}
	return false;
}

void UCInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UCInventoryWidget* InventoryWidget = Cast<UCInventoryWidget>(ParentWidget);
	if (CurrentItemID != EItemID::None && InventoryWidget)
	{
		FVector2D MousePoseition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		InventoryWidget->ShowExplainWidget(CurrentSlotData, MousePoseition);
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
