#include "UI/CCropWidget.h"
#include "Crops/CBase_Crop.h"
#include "Components/CNutritionComponent.h"
#include "Components/CMoistureComponent.h"
#include "Components/CHealthComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/CStateDisplayWidget.h"

void UCCropWidget::SetCrop_Implementation(ACBase_Crop* InCrop)
{
	Crop = InCrop;

	UCNutritionComponent* NutritionComp = Crop->GetNutritionComp();
	if (NutritionComp)
	{
		NutritionComp->OnStateValueChanged.AddDynamic(this, &UCCropWidget::UpdateNutrition);
		UpdateNutrition(0.0f, NutritionComp->GetCurrentValue(), NutritionComp->GetMaxValue());
	}

	UCMoistureComponent* MoistureComp = Crop->GetMoistureComp();
	if (MoistureComp)
	{
		MoistureComp->OnStateValueChanged.AddDynamic(this, &UCCropWidget::UpdateMoisture);
		UpdateMoisture(0.0f, MoistureComp->GetCurrentValue(), MoistureComp->GetMaxValue());
	}

	UCHealthComponent* HealthComp = Crop->GetHealthComp();
	if (HealthComp)
	{
		HealthComp->OnStateValueChanged.AddDynamic(this, &UCCropWidget::UpdateHealth);
		UpdateHealth(0.0f, HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
	}
	if (Crop)
	{
		Crop->OnGrowthChanged.AddDynamic(this, &UCCropWidget::UpdateGrowth);
		UpdateGrowth(0.0f, Crop->GetCurretnGrowth(), 100.0f);
	}
}

void UCCropWidget::ResetCrop_Implementation()
{
	if (Crop)
	{
		Crop->GetNutritionComp()->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateNutrition);
		Crop->GetMoistureComp()->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateMoisture);
		Crop->GetHealthComp()->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateHealth);

		Crop = nullptr;
	}
}

void UCCropWidget::UpdateNutrition_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (NutritionState)
	{
		NutritionState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCCropWidget::UpdateMoisture_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (MoistureState)
	{
		MoistureState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCCropWidget::UpdateHealth_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (HealthState)
	{
		HealthState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCCropWidget::UpdateGrowth_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (GrowthState)
	{
		GrowthState->UpdateStateDisplay(NewValue, MaxValue);
	}
}
