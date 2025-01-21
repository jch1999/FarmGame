#include "UI/CFarmFieldWidget.h"
#include "Farm/CFarmField.h"
#include "Components/ProgressBar.h"
#include "Components/CCultivationComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CMoistureComponent.h"

void UCFarmFieldWidget::SetFarmField_Implementation(ACFarmField* InFarmField)
{
	FarmField = InFarmField;

	UCCultivationComponent* CultivationComp = FarmField->GetCultivationComp();
	if (CultivationComp)
	{
		CultivationComp->OnCultivationChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateCultivation);
		UpdateCultivation(0.0f, CultivationComp->GetCurrentCultivation(), CultivationComp->GetMaxCultivation());
	}

	UCNutritionComponent* NutritionComp= FarmField->GetNutritionComp();
	if (NutritionComp)
	{
		NutritionComp->OnNutritionChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateNutrition);
		UpdateNutrition(0.0f, NutritionComp->GetCurrentNutrition(), NutritionComp->GetMaxNutrition());
	}

	UCMoistureComponent* MoistureComp = FarmField->GetMoistureComp();
	if (MoistureComp)
	{
		FarmField->GetMoistureComp()->OnMoistureChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateMoisture);
		UpdateMoisture(0.0f, MoistureComp->GetCurrentMoisture(), MoistureComp->GetMaxMoisture());
	}
}

void UCFarmFieldWidget::ResetFarmField_Implementation()
{
	FarmField->GetCultivationComp()->OnCultivationChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateCultivation);
	FarmField->GetNutritionComp()->OnNutritionChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateNutrition);
	FarmField->GetMoistureComp()->OnMoistureChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateMoisture);

	FarmField = nullptr;
}

void UCFarmFieldWidget::UpdateCultivation_Implementation(float OldValue, float NewValue, float MaxValue)
{
	CultivationBar->SetPercent(NewValue / MaxValue);
}

void UCFarmFieldWidget::UpdateNutrition_Implementation(float OldValue, float NewValue, float MaxValue)
{
	NutritionBar->SetPercent(NewValue / MaxValue);
}

void UCFarmFieldWidget::UpdateMoisture_Implementation(float OldValue, float NewValue, float MaxValue)
{
	MositureBar->SetPercent(NewValue / MaxValue);
}
