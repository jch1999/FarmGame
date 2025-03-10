#pragma once

#include "CoreMinimal.h"
#include "UI/CSlotWidget.h"
#include "CInventorySlotWidget.generated.h"

class UCInventoryWidget;
class UCSlotDropDownWidget;
class UUserWidget;

UCLASS()
class FARMGAME_API UCInventorySlotWidget : public UCSlotWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// About Hover
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	// DropDown
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void OpenDropDownMenu();

	virtual void SetParentWidget(UUserWidget* InParent) override;

private:
	void HideEplainWidget();

protected:
	UPROPERTY()
	UCSlotDropDownWidget* SlotDropDownWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCSlotDropDownWidget> SlotDropDownWidgetClass;

private:
	FTimerHandle HideExplainTimer;
};
