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
		NutritionComp->OnNutritionChanged.AddDynamic(this, &UCCropWidget::UpdateNutrition);
		UpdateNutrition(0.0f, NutritionComp->GetCurrentNutrition(), NutritionComp->GetMaxNutrition());
	}

	UCMoistureComponent* MoistureComp = Crop->GetMoistureComp();
	if (MoistureComp)
	{
		MoistureComp->OnMoistureChanged.AddDynamic(this, &UCCropWidget::UpdateMoisture);
		UpdateMoisture(0.0f, MoistureComp->GetCurrentMoisture(), MoistureComp->GetMaxMoisture());
	}

	UCHealthComponent* HealthComp = Crop->GetHealthComp();
	if (HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &UCCropWidget::UpdateHealth);
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
		Crop->GetNutritionComp()->OnNutritionChanged.RemoveDynamic(this, &UCCropWidget::UpdateNutrition);
		Crop->GetMoistureComp()->OnMoistureChanged.RemoveDynamic(this, &UCCropWidget::UpdateMoisture);
		Crop->GetHealthComp()->OnHealthChanged.RemoveDynamic(this, &UCCropWidget::UpdateHealth);

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
