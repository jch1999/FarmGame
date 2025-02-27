#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CQuickSlotBarWidget.generated.h"

class UUniformGridPanel;
class UCQuickSlotWidget;

UCLASS()
class FARMGAME_API UCQuickSlotBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* QuickSlotGridPanel;

	UPROPERTY(EditDefaultsOnly,Category="UI|Class")
	TSubclassOf<UCQuickSlotWidget> QuickSlotClass;

};
