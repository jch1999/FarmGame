#include "UI/CQuickSlotWidget.h"
#include "UI/CSlotWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CInventorySlotDragDropOperation.h"
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

	return true;
}

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
		ParentWidget = QuickSlotBar;
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
			switch(DragOperation->SourceSlot->SlotType)
			{
			case ESlotType::Inventory_Player:
			{
				SetItem(*(DragOperation->SourceSlot->GetSlotItemData()));
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

			DragOperation->SourceSlot->ItemIconImage->SetOpacity(1.0f);
			if (UGameInstance* GI = GetWorld()->GetGameInstance<UCGameInstance>())
			{
				if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
				{
					MyGI->StopDragging();
				}
			}
			return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
		}
	}
	return false;
}