#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDrawConnetLineWidget.generated.h"

UCLASS()
class FARMGAME_API UCDrawConnetLineWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D EndPoint;

protected:
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;
};
