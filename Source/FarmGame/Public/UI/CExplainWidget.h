#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CExplainWidget.generated.h"

class ICItemInterface;
class UImage;
class UTextBlock;

UCLASS()
class FARMGAME_API UCExplainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItem(const FInventorySlot& InSlotData);

public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemExtraText; // Seed의 경우 종류 출력, Crop의 경우 품질이 출력됨
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

};
