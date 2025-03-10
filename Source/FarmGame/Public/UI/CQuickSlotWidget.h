#pragma once

#include "CoreMinimal.h"
#include "UI/CSlotWidget.h"
#include "CQuickSlotWidget.generated.h"

class UTextBlock;
class UCQuickSlotBarWidget;

UCLASS()
class FARMGAME_API UCQuickSlotWidget : public UCSlotWidget
{
	GENERATED_BODY()
	
public:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable)
	bool SetQuickSlotIndex(int32 InIndex);

	void SetParentWidget(UUserWidget* InParent) override;

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuickSlotIndexText;

	UCQuickSlotBarWidget* ParentQuickSlotBarWidget;
};
