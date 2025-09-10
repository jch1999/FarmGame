#include "UI/CQuickSlotWidget.h"
#include "UI/CSlotWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CSlotDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CGameInstance.h"
#include "Controller/CPlayerController.h"
#include "Characters/CPlayer.h"

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
	if (InIndex < 0 || InIndex > 10) return false;
	
	int32 Index = (InIndex + 1) % 10;
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
			UE_LOG(LogTemp, Warning, TEXT("QuickSlot Inserted!"));
			switch(DragOperation->SourceSlot->GetSlotType())
			{
			case ESlotType::Inventory_Player:
			{
				TargetSlotIndex = DragOperation->SourceSlot->GetSlotIndex();
				UE_LOG(LogTemp, Warning, TEXT("TargetSlotIndex : %d"), TargetSlotIndex);
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

			if (UCQuickSlotBarWidget* Bar = Cast<UCQuickSlotBarWidget>(ParentWidget))
			{
				UE_LOG(LogTemp, Warning, TEXT("Bar->CurrentIndex : %d , SlotIndex : %d"), Bar->CurrentIndex, SlotIndex);

				// 현재 선택된 퀵슬롯이면 아이템 액터 부착
				if (Bar->CurrentIndex==SlotIndex)
				{
					if (ACPlayerController* PC = GetOwningPlayer<ACPlayerController>())
					{
						if (ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn()))
						{
							UE_LOG(LogTemp, Warning, TEXT("Item Attached!"));

							Player->EquipItemFromQuickSlot(SlotIndex);
						}
					}
				}
			}
			return bHandled;
		}
	}
	return bHandled;
}