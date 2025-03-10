#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CInventoryWidget.generated.h"

class UCExplainWidget;
class UCInventorySlotWidget;
class UCInventoryComponent;
class UUniformGridPanel;
class UButton;
class UBorder;
class UCTitleBarWidget;
class UCStateDisplayWidget;

UCLASS()
class FARMGAME_API UCInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Initialize - Initialization function that is first called since the Constructor of UserWidget
	virtual bool Initialize() override;
	// After Initialize - Safe to use since all widgets and variables have been initialized
	virtual void NativeOnInitialized() override;
	/*virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;*/

public:
	void ShowExplainWidget(TWeakPtr<FInventorySlot> InSlotData, FVector2D ViewPortPosition);
	void HideExplainWidget();

	UFUNCTION()
	void UpdateInventorySlotWidget(const TArray<int32>& ChangedIndexs);

	UFUNCTION()
	void UpdateInventorySlotCount(int32 SlotIndex);

	UFUNCTION()
	void UpdateMoneyAmount(int32 InNewAmount);

	UFUNCTION()
	void SetInventoryComp(UCInventoryComponent* InComp);

	bool IsInExpainWidget();

private:
	void SetMoneyTextLerp();

public:
	UPROPERTY(meta = (BindWidget))
	UCTitleBarWidget* TitleBarWidget;

	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* MoneyDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|GridPanel")
	int32 GridPanelRow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|GridPanel")
	int32 GridPanelCol;

	const int32 MaxColumn = 5;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|Slot")
	TSubclassOf<UCInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Widget|Explain")
	TArray<UCInventorySlotWidget*> Slots;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget|Explain")
	TSubclassOf<UCExplainWidget> ExplainWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Widget|Explain")
	UCExplainWidget* ExplainWidget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Owner")
	UCInventoryComponent* InventoryComp;

private:
	FTimerHandle MoneyTextTimer;
	int32 CurrentMoney;
	int32 TargetMoney;
};
