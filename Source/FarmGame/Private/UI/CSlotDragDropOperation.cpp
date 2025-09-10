#include "UI/CSlotDragDropOperation.h"
#include "UI/CSlotWidget.h"
#include "Controller/CPlayerController.h"
#include "Components/Image.h"
#include "Characters//CPlayer.h"
#include "Components/CInventoryComponent.h"

void UCSlotDragDropOperation::Initialize(UCSlotWidget* InSourceSlot)
{
	SourceSlot = InSourceSlot;

	OnDragged.AddDynamic(this, &UCSlotDragDropOperation::DragStart);
	OnDragCancelled.AddDynamic(this, &UCSlotDragDropOperation::DragCancel);
	OnDrop.AddDynamic(this, &UCSlotDragDropOperation::DragEnd);
}

void UCSlotDragDropOperation::DragStart(UDragDropOperation* InOperation)
{
	
	if (SourceSlot)
	{
		if (UImage* ItemIconImage = SourceSlot->GetItemIconImage())
		{
			ItemIconImage->SetOpacity(0.3f);
			if (UUserWidget* OwningWidget = SourceSlot->GetTypedOuter<UUserWidget>())
			{
				if (APlayerController* PC = OwningWidget->GetOwningPlayer<APlayerController>())
				{
					if (ACPlayerController* MyPC = Cast<ACPlayerController>(PC))
					{
						MyPC->StartDragging(SourceSlot->GetSlotData()->ItemIcon);
					}
				}
			}
		}
	}

}

void UCSlotDragDropOperation::DragCancel(UDragDropOperation* InOperation)
{
	if (SourceSlot && SourceSlot->GetSlotType() == ESlotType::Inventory_Player)
	{
		if(UImage* ItemIconImage = SourceSlot->GetItemIconImage())
		{
			ItemIconImage->SetOpacity(1.0f);
		}
		if (UUserWidget* OwningWidget = SourceSlot->GetTypedOuter<UUserWidget>())
		{
			if (APlayerController* PC = OwningWidget->GetOwningPlayer<APlayerController>())
			{
				if (ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn()))
				{
					if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
					{
						InventoryComp->DropItem(SourceSlot->GetSlotIndex());
					}
				}

				if (ACPlayerController* MyPC = Cast<ACPlayerController>(PC))
				{
					MyPC->StopDragging();
				}
			}
		}
	}
}

void UCSlotDragDropOperation::DragEnd(UDragDropOperation* InOperation)
{
	if (SourceSlot)
	{
		if (UImage* ItemIconImage = SourceSlot->GetItemIconImage())
		{
			ItemIconImage->SetOpacity(1.0f);
		}
		if (UUserWidget* OwningWidget = SourceSlot->GetTypedOuter<UUserWidget>())
		{
			if (APlayerController* PC = OwningWidget->GetOwningPlayer<APlayerController>())
			{
				if (ACPlayerController* MyPC = Cast<ACPlayerController>(PC))
				{
					MyPC->StopDragging();
				}
			}
		}
	}
}
