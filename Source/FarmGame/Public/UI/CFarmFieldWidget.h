#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFarmFieldWidget.generated.h"

class ACFarmField;
class UProgressBar;

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

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="FarmField")
	ACFarmField* FarmField;

	// Info Progress Bar
	UPROPERTY(meta = (BindWidget))
	UProgressBar* CultivationBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* NutritionBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MoistureBar;
};
