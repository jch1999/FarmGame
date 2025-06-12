#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CMoistureComponent.h"
#include "Components/CNutritionComponent.h"
#include "CFarmFieldWidget.generated.h"

class ACFarmField;
class UProgressBar;
class UCStateDisplayWidget;
class UCanvasPanel;
class UImage;
class UButton;

UCLASS()
class FARMGAME_API UCFarmFieldWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetFarmField(ACFarmField* InFarmField);

	UFUNCTION(BlueprintNativeEvent)
	void ResetFarmField(float CameraDelayTime);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateCultivation(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateNutrition(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMoisture(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void OnNutritionStateChanged(ENutritionState InState);

	UFUNCTION(BlueprintNativeEvent)
	void OnMoistureStateChanged(EMoistureState InState);

	UFUNCTION()
	void CheckPlantBtnActive(int32 InIndex);

	void PositionStateDisplays();
	//void DrawConnectionLines();

	UButton* GetPlantBtn() { return PlantBtn; }

private:
	UFUNCTION()
	void OnPlantClicked();

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
	/*UPROPERTY(meta = (BindWidget))
	UImage* Line_Cultivate;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Nutrition;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Moisture;*/

	UPROPERTY(meta = (BindWidget))
	UButton* PlantBtn;

	UPROPERTY(EditDefaultsOnly, Category="Display")
	FVector2D Offset;
};
