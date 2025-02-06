#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

class UCHUDWidget;

UCLASS()
class FARMGAME_API ACHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="UI")
	TSubclassOf<UCHUDWidget> UIHudWidgetClass;

	UPROPERTY(BlueprintReadWrite,Category="UI")
	UCHUDWidget* UIHudWidget;

public:
	void CreateHUD();
	UCHUDWidget* GetHUD();
};
