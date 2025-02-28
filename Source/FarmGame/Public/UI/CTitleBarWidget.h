#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTitleBarWidget.generated.h"

class UButton;

UCLASS()
class FARMGAME_API UCTitleBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetParentWidget(UUserWidget* ParentWidget);
	FVector2D ScreenSpaceToViewport(FVector2D ScreenPosition);

public:
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;

	UPROPERTY(VisibleAnywhere);
	UUserWidget* ParentWidget;

	bool bIsDragging = false;
	FVector2D DragOffset;
};
