#include "UI/CDragIconWidget.h"
#include "Components/Image.h"

void UCDragIconWidget::SetItemIcon(UTexture2D* InIcon)
{
    if (IconImage && InIcon)
    {
        IconImage->SetBrushFromTexture(InIcon);
    }
}
