#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"


FReply UCTitleBarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        bIsDragging = true;
        //FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition(); 
        
        FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
        FVector2D WidgetPosition= ParentWidget->GetCachedGeometry().GetAbsolutePosition();
        DragOffset = MousePosition - WidgetPosition;
        UE_LOG(LogTemp, Warning, TEXT("MousePosition: %s, WidgetPosition: %s, DragOffset: %s"),
            *MousePosition.ToString(), *WidgetPosition.ToString(), *DragOffset.ToString());

        return FReply::Handled().CaptureMouse(TakeWidget());
        // TakeWidget(): Switch to UWidget or UserWidget -> SWidget (Slate-based widget).
        // CaptureMouse only works with Slate-based widgets
        // CaptureMouse(): A function that sets a particular widget to "capture" mouse input.
        // GetCachedGeometry() : Returns the location and size of the widget calculated from the previous frame
        // GetTickSpaceGeometry() : Gets the latest updated location and size from the current frame.
    }
    return FReply::Unhandled();
}

FReply UCTitleBarWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && HasMouseCapture() && ParentWidget)
    {
        FVector2D ViewportSize;
        GEngine->GameViewport->GetViewportSize(ViewportSize);
        //FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
        FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
        FVector2D WidgetSize = ParentWidget->GetCachedGeometry().GetLocalSize();
        FVector2D NewPosition = MousePosition - DragOffset; // Global Mouse Pos
        NewPosition.X = FMath::Clamp(NewPosition.X, 0.0f, ViewportSize.X - WidgetSize.X);
        NewPosition.Y = FMath::Clamp(NewPosition.Y, 0.0f, ViewportSize.Y -  WidgetSize.Y);
        ParentWidget->SetPositionInViewport(NewPosition, false);
      //  ParentWidget->SetPositionInViewport(MousePosition, false);
        UE_LOG(LogTemp, Warning, TEXT("Drag Start - MousePosition: %s, DragOffset: %s"),
            *NewPosition.ToString(), *DragOffset.ToString());

        //ParentWidget->SetRenderTranslation(NewPosition);
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
