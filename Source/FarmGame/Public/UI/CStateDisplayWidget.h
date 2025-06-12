#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStateDisplayWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class FARMGAME_API UCStateDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetAmountText(int32 InAmount);

	UFUNCTION(BlueprintCallable)
	void SetStateIconTexture(UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable)
	void SetStateIconColor(const FLinearColor& InColor);
	//void SetStateIcon(UTexture2D* InTexture, const FLinearColor& InColor);
	
	UFUNCTION(BlueprintCallable)
	void UpdateStateDisplay(float Value, float MaxValue);

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StateProgressBar;
};
