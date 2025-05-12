#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CExplainWidget.generated.h"

class ICItemInterface;
class UImage;
class UTextBlock;
class UScrollBox;

UCLASS()
class FARMGAME_API UCExplainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	void SetItem(TWeakPtr<FInventorySlot> InSlotData);
	void HideWidget();

public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemExtraText; // Seed의 경우 종류 출력, Crop의 경우 품질이 출력됨
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemDescriptionScrollBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemPriceText;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMouseOver;

private:
	FTimerHandle HideTimer;
};
