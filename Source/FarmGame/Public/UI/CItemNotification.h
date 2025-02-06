#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CItemNotification.generated.h"

class UTextBlock;
class UTexture2D;
class UImage;
UCLASS()
class FARMGAME_API UCItemNotification : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon);

public:
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_ItemName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemAmount;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Time")
	float LifeTime;
};
