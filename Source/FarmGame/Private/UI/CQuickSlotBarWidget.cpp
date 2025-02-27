#include "UI/CQuickSlotBarWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/CQuickSlotWidget.h"
#include "Global.h"

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
        }
    }
}
