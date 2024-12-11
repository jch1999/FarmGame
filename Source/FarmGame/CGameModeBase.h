#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCCropWidget;

UCLASS()
class FARMGAME_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UCCropWidget* GetCropWidget() { return CropWidget; }

private:
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UCCropWidget> CropWidgetClass;
	UCCropWidget* CropWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> FarmFieldWidgetClass;
	UUserWidget* FarmFieldWidget;
};
