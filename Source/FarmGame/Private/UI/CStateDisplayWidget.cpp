#include "UI/CStateDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UCStateDisplayWidget::SetAmountText(int32 InAmount)
{
	if (!AmountText) return;

	AmountText->SetText(FText::FromString(FString::FromInt(InAmount)));
}

void UCStateDisplayWidget::SetStateIconTexture(UTexture2D* InTexture)
{
	if (!IconImage || !InTexture) return;

	IconImage->SetBrushFromTexture(InTexture);
}

void UCStateDisplayWidget::SetStateIconColor(const FLinearColor& InColor)
{
	if (!IconImage) return;

	FSlateBrush Brush = IconImage->GetBrush();
	Brush.TintColor = FSlateColor(InColor);
	IconImage->SetBrush(Brush);
}

//void UCStateDisplayWidget::SetStateIcon(UTexture2D* InTexture, const FLinearColor& InColor)
//{
//	if (!IconImage) return;
//
//	FSlateBrush Brush;
//	Brush.SetResourceObject(InTexture);
//	Brush.TintColor = FSlateColor(InColor);
//
//	IconImage->SetBrush(Brush);
//}

void UCStateDisplayWidget::UpdateStateDisplay(float Value, float MaxValue)
{
	if (!StateProgressBar) return;

	const float Percent = (MaxValue > 0) ? (Value / MaxValue) : 0;
	StateProgressBar->SetPercent(Percent);

	if (AmountText)
	{
		AmountText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"), Value, MaxValue)));
	}
}