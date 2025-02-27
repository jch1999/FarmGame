#pragma once

#include "CoreMinimal.h"
#include "UI/CSlotWidget.h"
#include "CQuickSlotWidget.generated.h"

class UTextBlock;
UCLASS()
class FARMGAME_API UCQuickSlotWidget : public UCSlotWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool SetQuickSlotIndex(int32 InIndex);

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuickSlotIndexText;
};
