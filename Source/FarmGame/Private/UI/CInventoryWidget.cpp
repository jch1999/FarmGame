#include "UI/CInventoryWidget.h"
#include "Components/CInventoryComponent.h"
#include "UI/CExplainWidget.h"
#include "UI/CInventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "UI/CInventoryDragDropOperation.h"
#include "UI/CTitleBarWidget.h"
#include "UI/CStateDisplayWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Controller/CPlayerController.h"
#include "Global.h"

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
        TitleBarWidget->SetTitle("Player's Inventory", FLinearColor::Black);
        TitleBarWidget->SetParentWidget(this);
    }
    if (MoneyDisplay)
    {
        MoneyDisplay->SetAmountText(0);

        UTexture2D* CoinTex;
        CHelpers::GetAssetDynamic(&CoinTex, "/Game/ThirdParty/Icon/Icon_Money");
        if (CoinTex)
        {
            MoneyDisplay->SetStateIcon(CoinTex, FLinearColor::Yellow);
        }
    }
    if (!SlotWidgetClass)
    {
        CHelpers::GetClassDynamic(&SlotWidgetClass, "/Game/I/WB_CInventorySlot.WB_CInventorySlot_C");
    }
    if (!ExplainWidgetClass)
    {
        CHelpers::GetClassDynamic(&ExplainWidgetClass, "/Game/UI/WB_CExplainWidget.WB_CExplainWidget_C");
    }
    return true;
}

void UCInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    SetDesiredSizeInViewport(FVector2D(400, 300));
}

void UCInventoryWidget::ShowExplainWidget(TWeakPtr<FInventorySlot> InSlotData, FVector2D ViewPortPosition)
{
    if (!ExplainWidgetClass) return;
    if (!InSlotData.IsValid()) return;

    if (!ExplainWidget)
    {
        ExplainWidget = CreateWidget<UCExplainWidget>(this, ExplainWidgetClass, "ExplainWidget");
        ExplainWidget->AddToViewport();
    }
    ExplainWidget->SetItem(InSlotData);
    ExplainWidget->SetPositionInViewport(ViewPortPosition, false);
    ExplainWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCInventoryWidget::HideExplainWidget()
{
    if (!ExplainWidget) return;
    if (ExplainWidget->bIsMouseOver) return;
    ExplainWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCInventoryWidget::UpdateInventorySlotWidget(const TArray<int32>& ChangedIndexs)
{
    if (InventoryComp)
    {
        const TArray<FInventorySlot>& SlotDatas = InventoryComp->GetSlotDatas();
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

void UCInventoryWidget::UpdateInventorySlotCount(int32 SlotIndex)
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
                UCInventorySlotWidget* SlotWidget = CreateWidget<UCInventorySlotWidget>(this, SlotWidgetClass, FName(*SlotName));
                SlotWidget->SetParentWidget(this);
                SlotWidget->SetItem(SlotDatas[CurrentIndex + i]);
                SlotWidget->SlotIndex = CurrentIndex + i;
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
    }
}

void UCInventoryWidget::UpdateMoneyAmount(int32 InNewAmount)
{
    TargetMoney = InNewAmount;
    if (GetWorld()->GetTimerManager().TimerExists(MoneyTextTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(MoneyTextTimer);
    }
    GetWorld()->GetTimerManager().SetTimer(MoneyTextTimer, this, &UCInventoryWidget::SetMoneyTextLerp, 0.02f, true, 0.02f);
}

void UCInventoryWidget::SetInventoryComp(UCInventoryComponent* InComp)
{
    if (IsValid(InComp))
    {
        InventoryComp = InComp;
    }
}

bool UCInventoryWidget::IsInExpainWidget()
{
    if (ExplainWidget && ExplainWidget->bIsMouseOver == true)
    {
        return true;
    }

    return false;
}

void UCInventoryWidget::SetMoneyTextLerp()
{
    int32 PrevMoney = CurrentMoney;
    CurrentMoney = FMath::Lerp(CurrentMoney, TargetMoney, 0.2f);
    if (CurrentMoney == PrevMoney)
    {
        CurrentMoney = TargetMoney;
        GetWorld()->GetTimerManager().ClearTimer(MoneyTextTimer);
    }

    if (MoneyDisplay)
    {
        MoneyDisplay->SetAmountText(CurrentMoney);
    }
}
