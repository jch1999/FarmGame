#include "UI/CQuickSlotBarWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/CQuickSlotWidget.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Controller/CPlayerController.h"

void UCQuickSlotBarWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!QuickSlotClass)
    {
        CHelpers::GetClassDynamic(&QuickSlotClass, "/Game/UI/WB_CQuickSlot.WB_CQuickSlot_C");
    }
    if (QuickSlotGridPanel && QuickSlotClass)
    {
        for (int32 i = 1; i <= 10; i++)
        {
            UCQuickSlotWidget* NewSlot = CreateWidget<UCQuickSlotWidget>(GetOwningPlayer(), QuickSlotClass);
            NewSlot->SetQuickSlotIndex(i);
            QuickSlotGridPanel->AddChildToUniformGrid(NewSlot, 0, i - 1);
            NewSlot->SetItem(FInventorySlot());
            NewSlot->SetParentWidget(this);
            NewSlot->AddToViewport();
            QuickSlots.AddUnique(NewSlot);
        }
    }
}

void UCQuickSlotBarWidget::OnInitPlayer(ACPlayer* InPlayer)
{
    if (IsValid(InPlayer))
    {
        if (ACPlayerController* PC = Cast<ACPlayerController>(InPlayer->GetController()))
        {
            PC->OnQuickSlotSelectedDelegate.AddDynamic(this, &UCQuickSlotBarWidget::OnQuickSlotSelected);
        }
        if (UCInventoryComponent* InventoryComponent = InPlayer->GetInventoryComponent())
        {
            InventoryComp = InventoryComponent;
            InventoryComp->OnInventorySlotDataUpdated.AddDynamic(this, &UCQuickSlotBarWidget::FuncForBindUpdate);
            InventoryComp->OnInventorySlotSwap.AddDynamic(this, &UCQuickSlotBarWidget::SwapSlotData);
        }
    }
}

void UCQuickSlotBarWidget::SwapSlotData(int32 InIndex1, int32 InIndex2)
{
    for (auto QuickSlot : QuickSlots)
    {
        if (QuickSlot->TargetSlotIndex == -1) continue;

        if (QuickSlot->TargetSlotIndex == InIndex1)
        {
            QuickSlot->TargetSlotIndex = InIndex2;
        }
        else if (QuickSlot->TargetSlotIndex == InIndex2)
        {
            QuickSlot->TargetSlotIndex = InIndex1;
        }
    }
    UpdateQuickSlotDatas();
}

void UCQuickSlotBarWidget::OnQuickSlotSelected(int32 InIndex)
{
    if (QuickSlots.Num() > InIndex && InIndex > 0)
    {
        InIndex--;
        if (InIndex == CurrentIndex)
        {
            UE_LOG(LogItem, Error, TEXT("Same QuickSlot Index Selected: %d"), InIndex);
            return;
        }
        
        CurrentIndex = InIndex;
        if (ACPlayerController* PC = Cast<ACPlayerController>(GetOwningPlayer()))
        {
            if (ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn()))
            {
                Player->EquipItemFromQuickSlot(InIndex);
            }
        }

        UE_LOG(LogItem, Error, TEXT("Item Seletecd %s, QuickSlot Index : %d"), *(QuickSlots[InIndex]->GetName()),InIndex);
    }
}

const FInventorySlot& UCQuickSlotBarWidget::GetCurrentSlotData()
{
    return *(QuickSlots[CurrentIndex]->GetSlotItemData());
}

const UCQuickSlotWidget* const UCQuickSlotBarWidget::GetCurrentSlot()
{
    return QuickSlots[CurrentIndex];
}

void UCQuickSlotBarWidget::FuncForBindUpdate(const TArray<int32>& ChangedIndex)
{
    UpdateQuickSlotDatas();
}

void UCQuickSlotBarWidget::UpdateQuickSlotDatas()
{
    for (auto QuickSlot : QuickSlots)
    {
        if (QuickSlot->TargetSlotIndex == -1) continue;

        const TArray<FInventorySlot>& SlotDatas = InventoryComp->GetSlotDatas();
        QuickSlot->SetItem(SlotDatas[QuickSlot->TargetSlotIndex]);
        if (SlotDatas[QuickSlot->TargetSlotIndex].ItemID == EItemID::None)
        {
            QuickSlot->TargetSlotIndex = -1;
        }
    }
}
