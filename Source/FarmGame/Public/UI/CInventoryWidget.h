#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CInventoryWidget.generated.h"

class UCExplainWidget;
class UCSlotWidget;
class UCInventoryComponent;
class UUniformGridPanel;
class UButton;
class UBorder;
class UCTitleBarWidget;

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
	void ShowExplainWidget(TWeakPtr<FInventorySlot> InSlotData, FVector2D ScreenPosition);
	void HideExplainWidget();

	UFUNCTION()
	void UpdateInventorySlotWidget(const TArray<int32>& ChangedIndexs);

	UFUNCTION()
	void UpdateInventorySlotCount(int32 SlotIndex);

	UFUNCTION()
	void SetInventoryComp(UCInventoryComponent* InComp);

public:
	UPROPERTY(meta = (BindWidget))
	UCTitleBarWidget* TitleBarWidget;

	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|GridPanel")
	int32 GridPanelRow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|GridPanel")
	int32 GridPanelCol;

	const int32 MaxColumn = 5;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|Slot")
	TSubclassOf<UCSlotWidget> SlotWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Widget|Explain")
	TArray<UCSlotWidget*> Slots;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget|Explain")
	TSubclassOf<UCExplainWidget> ExplainWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Widget|Explain")
	UCExplainWidget* ExplainWidget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Owner")
	UCInventoryComponent* InventoryComp;
};
