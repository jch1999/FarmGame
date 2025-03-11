#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CInventoryComponent.h"
#include "CQuickSlotBarWidget.generated.h"

class UUniformGridPanel;
class UCQuickSlotWidget;
class ACPlayer;

UCLASS()
class FARMGAME_API UCQuickSlotBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void OnInitPlayer(ACPlayer* InPlayer);
	UFUNCTION()
	void OnQuickSlotSelected(int32 InIndex);

private:
	UFUNCTION()
	void SwapSlotData(int32 InIndex1, int32 InIndex2);

	UFUNCTION()
	void FuncForBindUpdate(const TArray<int32>& ChangedIndex);
	void UpdateQuickSlotDatas();
public:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* QuickSlotGridPanel;

	UPROPERTY(EditDefaultsOnly, Category="QuickSlots")
	TArray<UCQuickSlotWidget*> QuickSlots;

	UPROPERTY(EditDefaultsOnly, Category = "QuickSlots")
	int32 CurrentIndex;

	UPROPERTY(EditDefaultsOnly,Category="UI|Class")
	TSubclassOf<UCQuickSlotWidget> QuickSlotClass;

protected:
	UCInventoryComponent* InventoryComp;
};
