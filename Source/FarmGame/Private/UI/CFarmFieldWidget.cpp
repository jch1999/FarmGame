#include "UI/CFarmFieldWidget.h"
#include "Farm/CFarmField.h"
#include "Components/ProgressBar.h"
#include "Components/CCultivationComponent.h"
#include "Components/CNutritionComponent.h"
#include "Components/CMoistureComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "UI/CStateDisplayWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"

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
	Offset=FVector2D(100.0f, 80.0f);

	PositionStateDisplays();
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

void UCFarmFieldWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UCFarmFieldWidget::PositionStateDisplays()
{
	if (!CanvasPanel || !FarmField || !CultivateState || !NutritionState || !MoistureState) return;

	// FarmField의 화면 좌표를 가져옴
	FVector2D ScreenPosition;
	APlayerController* PC = GetOwningPlayer();
	if (PC && PC->ProjectWorldLocationToScreen(FarmField->GetActorLocation(), ScreenPosition))
	{
		// 중심을 FarmField의 화면 위치로 설정
		FVector2D CenterPosition = ScreenPosition;
		
		if (UCanvasPanelSlot* PlantBtnSlot = Cast<UCanvasPanelSlot>(PlantBtn->Slot))
		{
			PlantBtnSlot->SetPosition(CenterPosition);
		}
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

		// 선도 갱신
		DrawConnectionLines();
	}
}

void UCFarmFieldWidget::DrawConnectionLines()
{
	if (!FarmField || !Line_Cultivate || !Line_Nutrition || !Line_Moisture) return;

	FVector2D CenterPosition;
	APlayerController* PC = GetOwningPlayer();
	if (PC && PC->ProjectWorldLocationToScreen(FarmField->GetActorLocation(), CenterPosition))
	{
		FVector2D CultivatePos, NutritionPos, MoisturePos, dummy;
		auto geometry = CultivateState->GetCachedGeometry();
		USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), geometry.GetAbsolutePosition(), dummy, CultivatePos); 
		geometry = NutritionState->GetCachedGeometry();
		USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), geometry.GetAbsolutePosition(), dummy, NutritionPos);
		geometry = MoistureState->GetCachedGeometry();
		USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), geometry.GetAbsolutePosition(), dummy, MoisturePos);

		FVector2D LineSize(100.0f, 2.0f);

		if (UCanvasPanelSlot* CultivateLineSlot = Cast<UCanvasPanelSlot>(Line_Cultivate->Slot))
		{
			CultivateLineSlot->SetSize(LineSize);
			CultivateLineSlot->SetPosition((CenterPosition + CultivatePos) * 0.5f); // 중앙 위치
			FWidgetTransform Transform;
			Transform.Angle = FMath::Atan2(CultivatePos.Y - CenterPosition.Y, CultivatePos.X - CenterPosition.X) * (180.0f / PI);
			Line_Cultivate->SetRenderTransform(Transform);
		}

		if (UCanvasPanelSlot* NutritionLineSlot = Cast<UCanvasPanelSlot>(Line_Nutrition->Slot))
		{
			NutritionLineSlot->SetSize(LineSize);
			NutritionLineSlot->SetPosition((CenterPosition + NutritionPos) * 0.5f);
			FWidgetTransform Transform;
			Transform.Angle = FMath::Atan2(NutritionPos.Y - CenterPosition.Y, NutritionPos.X - CenterPosition.X) * (180.0f / PI);
			Line_Nutrition->SetRenderTransform(Transform);
		}

		if (UCanvasPanelSlot* MoistureLineSlot = Cast<UCanvasPanelSlot>(Line_Moisture->Slot))
		{
			MoistureLineSlot->SetSize(LineSize);
			MoistureLineSlot->SetPosition((CenterPosition + MoisturePos) * 0.5f);
			FWidgetTransform Transform;
			Transform.Angle = FMath::Atan2(MoisturePos.Y - CenterPosition.Y, MoisturePos.X - CenterPosition.X) * (180.0f / PI);
			Line_Moisture->SetRenderTransform(Transform);
		}
	}
}
