#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFarmFieldWidget.generated.h"

class ACFarmField;
class UProgressBar;
class UCStateDisplayWidget;
class UCanvasPanel;
class UImage;

UCLASS()
class FARMGAME_API UCFarmFieldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetFarmField(ACFarmField* InFarmField);

	UFUNCTION(BlueprintNativeEvent)
	void ResetFarmField();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateCultivation(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateNutrition(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMoisture(float OldValue, float NewValue, float MaxValue);


	void PositionStateDisplays();
	void DrawConnectionLines();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="FarmField")
	ACFarmField* FarmField;

	// Canvas
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	// State Display
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* CultivateState;
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* NutritionState;
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* MoistureState;

	// Under Lines
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Cultivate;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Nutrition;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Moisture;
};
