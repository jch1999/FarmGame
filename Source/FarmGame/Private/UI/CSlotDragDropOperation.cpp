#include "UI/CSlotDragDropOperation.h"
#include "UI/CSlotWidget.h"
#include "CGameInstance.h"
#include "Components/Image.h"

void UCSlotDragDropOperation::Initialize(UCSlotWidget* InSourceSlot)
{
	SourceSlot = InSourceSlot;

	OnDragged.AddDynamic(this, &UCSlotDragDropOperation::DragStart);
	OnDragCancelled.AddDynamic(this, &UCSlotDragDropOperation::DragEnd);
	OnDrop.AddDynamic(this, &UCSlotDragDropOperation::DragEnd);
}

void UCSlotDragDropOperation::DragStart(UDragDropOperation* InOperation)
{
	if (SourceSlot && SourceSlot->ItemIconImage)
	{
		SourceSlot->ItemIconImage->SetOpacity(0.3f); 
		if (UUserWidget* OwningWidget = SourceSlot->GetTypedOuter<UUserWidget>())
		{
			if (UWorld* World = OwningWidget->GetWorld())
			{
				if (UGameInstance* GI = World->GetGameInstance())
				{
					if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
					{
						MyGI->StartDragging(SourceSlot->CurrentSlotData->ItemIcon);
					}
				}
			}
		}
	}

	
}

void UCSlotDragDropOperation::DragEnd(UDragDropOperation* InOperation)
{
	if (SourceSlot && SourceSlot->ItemIconImage)
	{
		SourceSlot->ItemIconImage->SetOpacity(1.0f);

		if (UUserWidget* OwningWidget = SourceSlot->GetTypedOuter<UUserWidget>())
		{
			if (UWorld* World = OwningWidget->GetWorld())
			{
				if (UGameInstance* GI = World->GetGameInstance())
				{
					if (UCGameInstance* MyGI = Cast<UCGameInstance>(GI))
					{
						MyGI->StopDragging();
					}
				}
			}
		}
	}
}
