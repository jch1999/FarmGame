#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFarmInteractionContainerWidget.generated.h"

class USizeBox;
class UCFarmFieldWidget;
class UCCropWidget;
class UButton;
class ACFarmField;

UCLASS()
class FARMGAME_API UCFarmInteractionContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetFarmField(ACFarmField* Field);
	UFUNCTION()
	void ResetFarmField(float DelayTime);

	UCFarmFieldWidget* GetFarmFieldWidget() { return FarmFieldWidget; }
	UCCropWidget* GetCropWidget() { return CropWidget; }

	UButton* GetCloseBtn() { return CloseBtn; }
protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* FarmFieldSlot;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CropSlot;

	UPROPERTY(meta = (BindWidget))
	UCFarmFieldWidget* FarmFieldWidget;

	UPROPERTY(meta = (BindWidget))
	UCCropWidget* CropWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseBtn;

private:
	UPROPERTY()
	ACFarmField* TargetFarmField;
};
