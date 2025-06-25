#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CSlotDragDropOperation.generated.h"

class UCSlotWidget;
UCLASS()
class FARMGAME_API UCSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
    void Initialize(UCSlotWidget* InSourceSlot);

    UFUNCTION()
    void DragStart(UDragDropOperation* InOperation);

    UFUNCTION()
    void DragCancel(UDragDropOperation* InOperation);
    
    UFUNCTION()
    void DragEnd(UDragDropOperation* InOperation);

public:
    UPROPERTY()
    UCSlotWidget* SourceSlot;

};
