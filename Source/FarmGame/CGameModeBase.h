#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCCropWidget;
class UCFarmFieldWidget;

UCLASS()
class FARMGAME_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	UCCropWidget* GetCropWidget();

	UFUNCTION(BlueprintPure)
	UCFarmFieldWidget* GetFarmFieldWidget();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFarmValue() { return FarmValue; }

	UFUNCTION(BlueprintCallable)
	void AddFarmVale(float InAddAmount);

	UFUNCTION(BlueprintCallable)
	void ReduceFarmVale(float InReduceAmount);

private:
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UCCropWidget> CropWidgetClass;
	UCCropWidget* CropWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCFarmFieldWidget> FarmFieldWidgetClass;
	UCFarmFieldWidget* FarmFieldWidget;

	UPROPERTY(VisibleAnywhere,Category="Farm")
	float FarmValue;
};
