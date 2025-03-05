#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void OnQuickSlotSelected(int InIndex);

public:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* QuickSlotGridPanel;

	UPROPERTY(EditDefaultsOnly, Category="QuickSlots")
	TArray<UCQuickSlotWidget*> QuickSlots;

	UPROPERTY(EditDefaultsOnly,Category="UI|Class")
	TSubclassOf<UCQuickSlotWidget> QuickSlotClass;

};
