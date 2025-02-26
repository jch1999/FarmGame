#include "UI/CInventoryWidget.h"
#include "Components/CInventoryComponent.h"
#include "UI/CExplainWidget.h"
#include "UI/CSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "UI/CInventoryDragDropOperation.h"
#include "UI/CTitleBarWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Controller/CPlayerController.h"

bool UCInventoryWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    if (!bSuccess) return false;

    if (InventoryGridPanel)
    {
        InventoryGridPanel->SetSlotPadding(FMargin(5.0f));
    }
    if (TitleBarWidget)
    {
        TitleBarWidget->SetParentWidget(this);

        if (APlayerController* Controller = GetOwningPlayer())
        {
            if (ACPlayerController* MyController = Cast<ACPlayerController>(Controller))
            {
                TitleBarWidget->CloseButton->OnClicked.AddDynamic(MyController,&ACPlayerController::CloseInventoryForCloseBtn);
            }
        }
    }
    return true;
}

void UCInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    SetDesiredSizeInViewport(FVector2D(400, 300));
}

void UCInventoryWidget::ShowExplainWidget(TWeakPtr<FInventorySlot> InSlotData, FVector2D ScreenPosition)
{
    if (!ExplainWidgetClass) return;
    if (!InSlotData.IsValid()) return;

    if (!ExplainWidget)
    {
        ExplainWidget = CreateWidget<UCExplainWidget>(this, ExplainWidgetClass, "ExplainWidget");
    }
    ExplainWidget->SetItem(InSlotData);
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

void UCInventoryWidget::UpdateInventory(const TArray<int32>& ChangedIndexs)
{
    if (InventoryComp)
    {
        const TArray<FInventorySlot>& SlotDatas = InventoryComp->GetSlotDatas();
        if (SlotDatas.Num() > Slots.Num())
        {
            int32 NewSlotCnt = SlotDatas.Num() - Slots.Num();
            int32 CurrentIndex = Slots.Num();
            for (int32 i = 0; i < NewSlotCnt; i++)
            {
                FString SlotName = "Slot_" + FString::FromInt(Slots.Num() + 1);
                UCSlotWidget* SlotWidget = CreateWidget<UCSlotWidget>(this, SlotWidgetClass, FName(*SlotName));
                SlotWidget->SetItem(SlotDatas[CurrentIndex++]); 
                Slots.Add(SlotWidget);
                
                InventoryGridPanel->AddChildToUniformGrid(SlotWidget, GridPanelRow, GridPanelCol);
                GridPanelCol++;
                if (GridPanelCol >= MaxColumn)
                {
                    GridPanelCol = 0;
                    GridPanelRow++;
                }
            }
        }
        for (int32 i = 0; i < ChangedIndexs.Num(); i++)
        {
            int32 TargetIndex = ChangedIndexs[i];
            if (TargetIndex < Slots.Num() && TargetIndex >= 0)
            {
                Slots[TargetIndex]->SetItem(SlotDatas[TargetIndex]);
            }
        }
    }
}

FReply UCInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (TitleBarWidget->bIsDragging)
    {
        FVector2D NewPosition = FSlateApplication::Get().GetCursorPos() - TitleBarWidget->DragOffset; // Global Mouse Pos
        SetPositionInViewport(NewPosition, false);
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UCInventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (TitleBarWidget->bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        TitleBarWidget->bIsDragging = false;
        UE_LOG(LogTemp, Display, TEXT("MouseUp"));
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}