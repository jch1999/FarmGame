#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CInventoryDragDropOperation.generated.h"

UCLASS()
class FARMGAME_API UCInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
    UPROPERTY()
    UUserWidget* DraggedWidget;

    UPROPERTY()
    FVector2D DragOffset;
};
