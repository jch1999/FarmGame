#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"


FReply UCTitleBarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        bIsDragging = true;
        DragOffset = InMouseEvent.GetScreenSpacePosition() - ParentWidget->GetCachedGeometry().GetAbsolutePosition();// +GetCachedGeometry().GetAbsolutePosition();


        return FReply::Handled().CaptureMouse(TakeWidget());
        // TakeWidget(): Switch to UWidget or UserWidget -> SWidget (Slate-based widget).
        // CaptureMouse only works with Slate-based widgets
        // CaptureMouse(): A function that sets a particular widget to "capture" mouse input.
    }
    return FReply::Unhandled();
}

FReply UCTitleBarWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && HasMouseCapture() && ParentWidget)
    {
        FVector2D NewPosition = FSlateApplication::Get().GetCursorPos() - DragOffset; // Global Mouse Pos
        ParentWidget->SetPositionInViewport(NewPosition, false);
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UCTitleBarWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = false;
        UE_LOG(LogTemp, Display, TEXT("MouseUp"));
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}

void UCTitleBarWidget::SetParentWidget(UUserWidget* InParentWidget)
{
    ParentWidget = InParentWidget;
}
