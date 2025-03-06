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
class UCQuickSlotBarWidget;
class ACPlayer;

UCLASS()
class FARMGAME_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void OnInitPlayer(ACPlayer* InPlayer);

	// GetWidget
	UCInteractRowScroll* GetInteractRowScroll() { return InteractRowScroll; }
	UCQuickSlotBarWidget* GetQuickSlotBar() { return QuickSlotBar; }

	// Abuot Item
	UFUNCTION()
	void OnItemAdded(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon);
	bool AddItemNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon);


protected:
	// About Interact Variables
	UPROPERTY(BlueprintReadWrite, Category = "Interact", meta=(BindWidget))
	UCInteractRowScroll* InteractRowScroll;

	UPROPERTY(BlueprintReadWrite, Category = "QuickSlot", meta = (BindWidget))
	UCQuickSlotBarWidget* QuickSlotBar;

	// About Item
	UPROPERTY(BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	UVerticalBox* VB_ItemNotifications;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCItemNotification> ItemNotificationClass;

};
