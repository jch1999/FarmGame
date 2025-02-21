#include "UI/CInventoryWidget.h"
#include "Components/CInventoryComponent.h"
#include "UI/CExplainWidget.h"

void UCInventoryWidget::ShowExplainWidget(FInventorySlot SlotData, FVector2D ScreenPosition)
{
    if (!ExplainWidget) return;

    ExplainWidget->SetItem(SlotData);
    ExplainWidget->SetPositionInViewport(ScreenPosition);
    ExplainWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCInventoryWidget::HideExplainWidget()
{
    if (ExplainWidget)
    {
        ExplainWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}
