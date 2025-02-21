#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryWidget.generated.h"

class UCExplainWidget;
class UCInventoryComponent;

UCLASS()
class FARMGAME_API UCInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

public:
	void ShowExplainWidget(FInventorySlot SlotData, FVector2D ScreenPosition);
	void HideExplainWidget();

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget|Explain")
	TSubclassOf<UCExplainWidget> ExplainWidgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Widget|Explain")
	UCExplainWidget* ExplainWidget;
};
