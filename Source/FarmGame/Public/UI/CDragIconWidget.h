#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDragIconWidget.generated.h"

class UImage;

UCLASS()
class FARMGAME_API UCDragIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitDragIcon(UTexture2D* InIcon);

public:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;
};
