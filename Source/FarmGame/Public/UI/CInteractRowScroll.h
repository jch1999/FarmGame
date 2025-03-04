#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInteractRowScroll.generated.h"

class UScrollBox;
class UVerticalBox;
class UCInteractRow;
class UCItemNotification;
class UImage;
class ACPlayer;

UCLASS()
class FARMGAME_API UCInteractRowScroll : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	// About Interact Functions
	void UpInteractIndex();
	void DownInteractIndex();
	int32 GetInteractIndex();
	AActor* GetInteractTarget();
	void EnsureVisibleInteractRow();
	void ScrollLerp();

	UFUNCTION()
	void UpdateScroll(bool IsUp);
	UFUNCTION()
	void UpdateInteractInfo(const TArray<AActor*>& InteractableObjects);
	UFUNCTION()
	void DoInteract(ACPlayer* InteractCauser);

	// Etc
	float GetChildWidgetOffset(UScrollBox* ScrollBox, UWidget* TargetWidget);
	void OnInitPlyer(ACPlayer* InPlayer);

protected:
	// About Interact Variables
	UPROPERTY(BlueprintReadWrite, Category = "Interact", meta = (BindWidget))
	UScrollBox* InteractRowScroll;
	UPROPERTY(BlueprintReadWrite, Category = "Interact", meta = (BindWidget))
	UImage* MouseWheelIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	TSubclassOf<UCInteractRow> InteractRowClass;
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	TArray<UCInteractRow*> InteractRows;
	int32 InteractIdx;
	UCInteractRow* NowSelectedRow;
	FTimerHandle InteractScrollLerpTimer;
};
