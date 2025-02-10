#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUDWidget.generated.h"

class UScrollBox;
class UVerticalBox;
class UCInteractRow;
class UCItemNotification;

UCLASS()
class FARMGAME_API UCHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	// About Interact Functions
	bool AddInteractRow(AActor* InActor);
	bool RemoveInteractRow(AActor* InActor);
	void SetInteractIndex(int32 InIdx);
	void UpInteractIndex();
	void DownInteractIndex();
	int32 GetInteractIndex();
	AActor* GetInteractTarget();
	void EnsureVisibleInteractRow();
	void ScrollLerp();

	// Abuot Item
	bool AddItemNotification(FName InItemName, int32 InItemAmount, UTexture2D* InItemIcon);

	// Etc
	float GetChildWidgetOffset(UScrollBox* ScrollBox, UWidget* TargetWidget);

protected:
	// About Interact Variables
	UPROPERTY(BlueprintReadWrite, Category = "Interact", meta=(BindWidget))
	UScrollBox* InteractRowScroll;
	UPROPERTY(EditDefaultsOnly,Category="Interact")
	TSubclassOf<UCInteractRow> InteractRowClass;
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	TArray<UCInteractRow*> InteractRows;
	int32 InteractIdx;
	UCInteractRow* NowSelectedRow;
	FTimerHandle InteractScrollLerpTimer;

	// About Item
	UPROPERTY(BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	UVerticalBox* VB_ItemNotifications;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UCItemNotification> ItemNotificationClass;

};
