#include "UI/CDragIconWidget.h"
#include "Components/Image.h"

void UCDragIconWidget::InitDragIcon(UTexture2D* InIcon)
{
    if (IconImage && InIcon)
    {
        IconImage->SetBrushFromTexture(InIcon);
    }
    SetIsEnabled(false);
}
