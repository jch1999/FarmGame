#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSlotWidget.generated.h"

class ICItemInterface;
class UImage;
class UTextBlock;
class UCInventoryComponent;
class UCInventoryWidget;
class UCExplainWidget;
class UCSlotDropDownWidget;

UCLASS()
class FARMGAME_API UCSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetItem(const FInventorySlot& SlotData);

	// Drag Begin & Finish
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGemoetry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// About Hover
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	// DropDown
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void OpenDropDownMenu();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemIconImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemID CurrentItemID;

	UPROPERTY()
	FInventorySlot CurrentSlotData;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	UCInventoryComponent* InventoryComponent;

	UPROPERTY()
	UCInventoryWidget* ParentInventoryWidget;

	UPROPERTY()
	UCSlotDropDownWidget* SlotDropDownWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCSlotDropDownWidget> SlotDropDownWidgetClass;
};
