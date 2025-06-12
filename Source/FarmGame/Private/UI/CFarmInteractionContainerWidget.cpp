#include "UI/CFarmInteractionContainerWidget.h"
#include "Farm/CFarmField.h"
#include "Crops/CBase_Crop.h"
#include "UI/CFarmFieldWidget.h"
#include "UI/CCropWidget.h"
#include "Components/SizeBox.h"
#include "Farm/CFarmField.h"

void UCFarmInteractionContainerWidget::SetFarmField(ACFarmField* Field)
{
	if (FarmFieldSlot && FarmFieldWidget)
	{
		TargetFarmField = Field;
		FarmFieldSlot->SetVisibility(ESlateVisibility::Visible);
		FarmFieldWidget->SetFarmField(Field);
	}

	if (IsValid(Field->GetCrop()) && CropWidget && CropSlot)
	{
		CropSlot->SetVisibility(ESlateVisibility::Visible);
		CropWidget->SetCrop(Field->GetCrop());
	}
	else
	{
		CropSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCFarmInteractionContainerWidget::ResetFarmField(float DelayTime)
{
	if (FarmFieldSlot && FarmFieldWidget)
	{
		FarmFieldSlot->SetVisibility(ESlateVisibility::Visible);
		FarmFieldWidget->ResetFarmField(DelayTime);
	}
	if (IsValid(TargetFarmField->GetCrop()) && CropWidget && CropSlot)
	{
		CropWidget->ResetCrop();
	}
}
