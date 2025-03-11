#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CSlotWidget.generated.h"

class ICItemInterface;
class UImage;
class UTextBlock;
class UCInventoryWidget;
class UCExplainWidget;
class UCSlotDropDownWidget;
class UProgressBar;

UENUM()
enum class ESlotType :uint8
{
	Inventory_Player, Inventory_Other, Quick
};

UCLASS()
class FARMGAME_API UCSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetItem(const FInventorySlot& InSlotData);
	virtual void SetParentWidget(UUserWidget* InParent);

	FInventorySlot* GetSlotItemData();

	// Drag Begin & Finish
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGemoetry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemCountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemIconImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ItemDurabilityBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemID CurrentItemID;

	TSharedPtr<FInventorySlot> CurrentSlotData;

	UPROPERTY()
	int32 SlotIndex;


	UPROPERTY()
	ESlotType SlotType;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* ParentWidget;
};
