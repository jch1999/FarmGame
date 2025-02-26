#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"


FReply UCTitleBarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        bIsDragging = true;
        DragOffset = InMouseEvent.GetScreenSpacePosition() - ParentWidget->GetCachedGeometry().GetAbsolutePosition();


        return FReply::Handled().CaptureMouse(ParentWidget->TakeWidget());
        // TakeWidget(): Switch to UWidget or UserWidget -> SWidget (Slate-based widget).
        // CaptureMouse only works with Slate-based widgets
        // CaptureMouse(): A function that sets a particular widget to "capture" mouse input.
    }
    return FReply::Unhandled();
}

void UCTitleBarWidget::SetParentWidget(UUserWidget* InParentWidget)
{
    ParentWidget = InParentWidget;
}
