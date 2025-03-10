#include "UI/CQuickSlotWidget.h"
#include "Components/TextBlock.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"

bool UCQuickSlotWidget::SetQuickSlotIndex(int32 InIndex)
{
	if (!QuickSlotIndexText) return false;
	if (InIndex < 0 || InIndex >= 10) return false;
	
	QuickSlotIndexText->SetText(FText::FromString(FString::FromInt(InIndex)));
	return true;
}

void UCQuickSlotWidget::SetParentWidget(UUserWidget* InParent)
{
	if (UCQuickSlotBarWidget* QuickSlotBar = Cast< UCQuickSlotBarWidget>(InParent))
	{
		ParentQuickSlotBarWidget = QuickSlotBar;
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("QuickSlotWidget Error! Cast Failed UUser* InParent -> QickSlotBarWidget*"));
	}
}

bool UCQuickSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	if (UCInventorySlotDragDropOperation* DragOperation = Cast<UCInventorySlotDragDropOperation>(InOperation))
	{
		if (!DragOperation->SourceSlot)
		{
			UE_LOG(LogItem, Error, TEXT("SourceSlot is nullptr in UCQuickSlotWidget::NativeOnDrop"));
			return false;
		}
		if (DragOperation->SourceSlot != this)
		{
			SetItem(*(DragOperation->SourceSlot->GetSlotItemData()));
			return true;
		}
	}
	return false;
}