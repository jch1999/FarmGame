#include "UI/CQuickSlotWidget.h"
#include "Components/TextBlock.h"

bool UCQuickSlotWidget::SetQuickSlotIndex(int32 InIndex)
{
	if (!QuickSlotIndexText) return false;
	if (InIndex < 0 || InIndex >= 10) return false;
	
	QuickSlotIndexText->SetText(FText::FromString(FString::FromInt(InIndex)));
	return true;
}
