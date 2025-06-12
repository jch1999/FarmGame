#include "UI/CDrawConnetLineWidget.h"

int32 UCDrawConnetLineWidget::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	// 선 그리기
	TArray<FVector2D> Points;
	Points.Add(StartPoint);
	Points.Add(EndPoint);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Points,
		ESlateDrawEffect::None,
		FLinearColor::Green,
		true,
		2.0f
	);

	// 부모의 NativePaint도 호출
	return Super::NativePaint(
		Args,
		AllottedGeometry,
		MyCullingRect,
		OutDrawElements,
		LayerId + 1,
		InWidgetStyle,
		bParentEnabled
	);
}