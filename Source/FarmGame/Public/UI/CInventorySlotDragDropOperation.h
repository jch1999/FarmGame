#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CInventorySlotDragDropOperation.generated.h"

class UCSlotWidget;
UCLASS()
class FARMGAME_API UCInventorySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
    TSharedPtr<FInventorySlot> DraggedSlotData;

    UPROPERTY()
    UCSlotWidget* SourceSlot;
};
