#include "UI/CQuickSlotBarWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/CQuickSlotWidget.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Controller/CPlayerController.h"

void UCQuickSlotBarWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (QuickSlotGridPanel && QuickSlotClass)
    {
        for (int32 i = 1; i <= 10; i++)
        {
            UCQuickSlotWidget* NewSlot = CreateWidget<UCQuickSlotWidget>(GetOwningPlayer(), QuickSlotClass);
            NewSlot->SetQuickSlotIndex(i == 10 ? 0 : i);
            QuickSlotGridPanel->AddChildToUniformGrid(NewSlot, 0, i - 1);
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
    }
}

void UCQuickSlotBarWidget::OnQuickSlotSelected(int InIndex)
{
    if (QuickSlots.Num() < InIndex && InIndex > 0)
    {
        UE_LOG(LogItem, Error, TEXT("Item Seletecd %s, QuickSlot Index : %d"), *(QuickSlots[InIndex]->GetName()),InIndex);
    }
}
