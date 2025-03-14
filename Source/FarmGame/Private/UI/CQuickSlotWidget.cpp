#include "UI/CQuickSlotWidget.h"
#include "UI/CSlotWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CSlotDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CGameInstance.h"

bool UCQuickSlotWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SlotType = ESlotType::Quick;
	TargetSlotIndex = -1;

	return true;
}

bool UCQuickSlotWidget::SetQuickSlotIndex(int32 InIndex)
{
	if (!QuickSlotIndexText) return false;
	if (InIndex < 0 || InIndex >= 10) return false;
	
	int32 Index = InIndex % 10;
	QuickSlotIndexText->SetText(FText::FromString(FString::FromInt(Index)));
	SlotIndex = InIndex;
	return true;
}

void UCQuickSlotWidget::SetParentWidget(UUserWidget* InParent)
{
	if (UCQuickSlotBarWidget* QuickSlotBar = Cast< UCQuickSlotBarWidget>(InParent))
	{
		ParentWidget = QuickSlotBar;
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("QuickSlotWidget Error! Cast Failed UUser* InParent -> QickSlotBarWidget*"));
	}
}

bool UCQuickSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool bHandled = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (UCSlotDragDropOperation* DragOperation = Cast<UCSlotDragDropOperation>(InOperation))
	{
		if (!DragOperation->SourceSlot)
		{
			UE_LOG(LogItem, Error, TEXT("SourceSlot is nullptr in UCQuickSlotWidget::NativeOnDrop"));
			return bHandled;
		}
		if (DragOperation->SourceSlot != this)
		{
			switch(DragOperation->SourceSlot->SlotType)
			{
			case ESlotType::Inventory_Player:
			{
				SetItem(*(DragOperation->SourceSlot->GetSlotItemData()));
				TargetSlotIndex = DragOperation->SourceSlot->SlotIndex;
			}
				break;
			case ESlotType::Quick:
			{
				// QuickSlotBar Swap
			}
				break;
			default:
				break;
			}
			return bHandled;
		}
	}
	return bHandled;
}