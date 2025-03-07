#include "UI/CStateDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCStateDisplayWidget::SetAmountText(int32 InAmount)
{
	if (!AmountText) return;

	AmountText->SetText(FText::FromString(FString::FromInt(InAmount)));
}

void UCStateDisplayWidget::SetStateIcon(UTexture2D* InTexture, const FLinearColor& InColor)
{
	if (!IconImage) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(InTexture);
	Brush.TintColor = FSlateColor(InColor);

	IconImage->SetBrush(Brush);
}
