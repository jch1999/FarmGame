#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTitleBarWidget.generated.h"

UCLASS()
class FARMGAME_API UCTitleBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetParentWidget(UUserWidget* ParentWidget);

public:
	UPROPERTY(VisibleAnywhere);
	UUserWidget* ParentWidget;

	bool bIsDragging = false;
	FVector2D DragOffset;
};
