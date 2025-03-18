#include "UI/CFarmFieldWidget.h"
#include "Farm/CFarmField.h"
#include "Components/ProgressBar.h"
#include "Components/CCultivationComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CMoistureComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "UI/CStateDisplayWidget.h"

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

	PositionStateDisplays();
	DrawConnectionLines();
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
	if (CultivateState)
	{
		CultivateState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCFarmFieldWidget::UpdateNutrition_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (NutritionState)
	{
		NutritionState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCFarmFieldWidget::UpdateMoisture_Implementation(float OldValue, float NewValue, float MaxValue)
{
	if (MoistureState)
	{
		MoistureState->UpdateStateDisplay(NewValue, MaxValue);
	}
}

void UCFarmFieldWidget::PositionStateDisplays()
{
	if (!CanvasPanel || !CultivateState || !NutritionState || !MoistureState) return;

	FVector2D CenterPosition(150.0f, 150.0f);
	FVector2D Offset(120.0f, 100.0f);

	if (UCanvasPanelSlot* CultivateSlot = Cast<UCanvasPanelSlot>(CultivateState->Slot))
	{
		CultivateSlot->SetPosition(CenterPosition + FVector2D(-Offset.X, -Offset.Y));
	}

	if (UCanvasPanelSlot* NutritionSlot = Cast<UCanvasPanelSlot>(NutritionState->Slot))
	{
		NutritionSlot->SetPosition(CenterPosition + FVector2D(Offset.X, -Offset.Y));
	}

	if (UCanvasPanelSlot* MoistureSlot = Cast<UCanvasPanelSlot>(MoistureState->Slot))
	{
		MoistureSlot->SetPosition(CenterPosition + FVector2D(0, Offset.Y));
	}
}

void UCFarmFieldWidget::DrawConnectionLines()
{
	if (!Line_Cultivate || !Line_Nutrition || !Line_Moisture) return;

	FVector2D LineSize(100.0f, 2.0f);

	if (UCanvasPanelSlot* CultivateLineSlot = Cast<UCanvasPanelSlot>(Line_Cultivate->Slot))
	{
		CultivateLineSlot->SetSize(LineSize);
		CultivateLineSlot->SetPosition(FVector2D(110.0f, 100.0f));

		FWidgetTransform Transform;
		Transform.Angle = -45.0f;
		Line_Cultivate->SetRenderTransform(Transform);
	}

	if (UCanvasPanelSlot* NutritionLineSlot = Cast<UCanvasPanelSlot>(Line_Nutrition->Slot))
	{
		NutritionLineSlot->SetSize(LineSize);
		NutritionLineSlot->SetPosition(FVector2D(190.0f, 100.0f));
		
		FWidgetTransform Transform;
		Transform.Angle = 45.0f;
		Line_Cultivate->SetRenderTransform(Transform);
	}

	if (UCanvasPanelSlot* MoistureLineSlot = Cast<UCanvasPanelSlot>(Line_Moisture->Slot))
	{
		MoistureLineSlot->SetSize(LineSize);
		MoistureLineSlot->SetPosition(FVector2D(150.0f, 180.0f));
		
		FWidgetTransform Transform;
		Transform.Angle = 90.0f;
		Line_Moisture->SetRenderTransform(Transform);
	}
}
