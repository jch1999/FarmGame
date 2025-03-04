#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDWidget.generated.h"

class UScrollBox;
class UVerticalBox;
class UCInteractRow;
class UCItemNotification;
class UImage;
class UCInteractRowScroll;
class ACPlayer;

UCLASS()
class FARMGAME_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:

	// Abuot Item
	bool AddItemNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon);

	void OnInitPlayer(ACPlayer* InPlayer);
	UCInteractRowScroll* GetInteractRowScroll() { return InteractRowScroll; }

protected:
	// About Interact Variables
	UPROPERTY(BlueprintReadWrite, Category = "Interact", meta=(BindWidget))
	UCInteractRowScroll* InteractRowScroll;

	// About Item
	UPROPERTY(BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	UVerticalBox* VB_ItemNotifications;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCItemNotification> ItemNotificationClass;

};
