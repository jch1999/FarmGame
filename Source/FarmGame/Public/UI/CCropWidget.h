#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCropWidget.generated.h"

class ACBase_Crop;
class UCStateDisplayWidget;
class UButton;
class UImage;

UCLASS()
class FARMGAME_API UCCropWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetCrop(ACBase_Crop* InCrop);

	UFUNCTION(BlueprintNativeEvent)
	void ResetCrop();
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateNutrition(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMoisture(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealth(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateGrowth(float OldValue, float NewValue, float MaxValue);

	UFUNCTION(BlueprintNativeEvent)
	void OnNutritionStateChanged(ENutritionState InState);

	UFUNCTION(BlueprintNativeEvent)
	void OnMoistureStateChanged(EMoistureState InState);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE  bool IsAvailable() { return Crop == nullptr; }

	/*UButton* GetHarvestBtn() { return HarvestBtn; }*/
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Crop")
	ACBase_Crop* Crop;

	// State Display
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* NutritionState;
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* MoistureState;
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* HealthState;
	UPROPERTY(meta = (BindWidget))
	UCStateDisplayWidget* GrowthState;

	// Under Lines
	/*UPROPERTY(meta = (BindWidget))
	UImage* Line_Nutrition;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Moisture;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Health;
	UPROPERTY(meta = (BindWidget))
	UImage* Line_Growth;*/

	/*UPROPERTY(meta = (BindWidget))
	UButton* HarvestBtn;*/

	UPROPERTY(EditDefaultsOnly, Category = "Display")
	FVector2D Offset;
};
