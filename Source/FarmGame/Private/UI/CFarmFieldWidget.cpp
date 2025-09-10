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
#include "CHUD.h"
#include "UI/CHUDWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "UI/CQuickSlotWidget.h"
#include "Characters/CPlayer.h"
#include "Global.h"

void UCFarmFieldWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (CultivateState)
	{
		UTexture2D* CultivateIcon;
		CHelpers::GetAssetDynamic(&CultivateIcon, "/Game/ThirdParty/Icon/Icon_Agriculture");
		CultivateState->SetStateIconTexture(CultivateIcon);
		CultivateState->SetStateIconColor(FLinearColor(150.0f, 75.0f, 0.0f));
	}

	if (NutritionState)
	{
		UTexture2D* NutritionIcon;
		CHelpers::GetAssetDynamic(&NutritionIcon, "/Game/ThirdParty/Icon/Icon_Nutition");
		NutritionState->SetStateIconTexture(NutritionIcon);
		NutritionState->SetStateIconColor(FLinearColor::Red);
	}
	
	if (MoistureState)
	{
		UTexture2D* MoistureIcon;
		CHelpers::GetAssetDynamic(&MoistureIcon, "/Game/ThirdParty/Icon/Icon_Humidity_Mid");
		MoistureState->SetStateIconTexture(MoistureIcon);
		MoistureState->SetStateIconColor(FLinearColor::Red);
	}

	if (PlantBtn)
	{
		PlantBtn->SetIsEnabled(false);
		PlantBtn->OnClicked.AddDynamic(this, &UCFarmFieldWidget::OnPlantClicked);
	}
}

void UCFarmFieldWidget::SetFarmField_Implementation(ACFarmField* InFarmField)
{
	FarmField = InFarmField;

	UCCultivationComponent* CultivationComp = FarmField->GetCultivationComp();
	if (CultivationComp)
	{
		CultivationComp->OnStateValueChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateCultivation);
		UpdateCultivation(0.0f, CultivationComp->GetCurrentValue(), CultivationComp->GetMaxValue());
		UE_LOG(LogTemp, Warning, TEXT("Cultivation"));
	}

	UCNutritionComponent* NutritionComp= FarmField->GetNutritionComp();
	if (NutritionComp)
	{
		NutritionComp->OnStateValueChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateNutrition);
		NutritionComp->OnNutritionStateChanged.AddDynamic(this, &UCFarmFieldWidget::OnNutritionStateChanged);
		UpdateNutrition(0.0f, NutritionComp->GetCurrentValue(), NutritionComp->GetMaxValue());
		OnNutritionStateChanged(NutritionComp->GetCurrentState());
		UE_LOG(LogTemp, Warning, TEXT("Nutrition"));
	}

	UCMoistureComponent* MoistureComp = FarmField->GetMoistureComp();
	if (MoistureComp)
	{
		MoistureComp->OnStateValueChanged.AddDynamic(this, &UCFarmFieldWidget::UpdateMoisture);
		MoistureComp->OnMoistureStateChanged.AddDynamic(this, &UCFarmFieldWidget::OnMoistureStateChanged);
		UpdateMoisture(0.0f, MoistureComp->GetCurrentValue(), MoistureComp->GetMaxValue());
		OnMoistureStateChanged(MoistureComp->GetCurrentState());
		UE_LOG(LogTemp, Warning, TEXT("Moisture"));
	}
	Offset=FVector2D(100.0f, 80.0f);

	//PositionStateDisplays();
}

void UCFarmFieldWidget::ResetFarmField_Implementation(float CameraDelayTime)
{
	if (FarmField)
	{
		UCCultivationComponent* CultivationComp = FarmField->GetCultivationComp();
		if (CultivationComp)
		{
			CultivationComp->OnStateValueChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateCultivation);
		}

		UCNutritionComponent* NutritionComp = FarmField->GetNutritionComp();
		if (NutritionComp)
		{
			NutritionComp->OnStateValueChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateNutrition);
			NutritionComp->OnNutritionStateChanged.RemoveDynamic(this, &UCFarmFieldWidget::OnNutritionStateChanged);
		}
		UCMoistureComponent* MoistureComp = FarmField->GetMoistureComp();
		if (MoistureComp)
		{
			MoistureComp->OnStateValueChanged.RemoveDynamic(this, &UCFarmFieldWidget::UpdateMoisture);
			MoistureComp->OnMoistureStateChanged.RemoveDynamic(this, &UCFarmFieldWidget::OnMoistureStateChanged);
		}
		FarmField->SetDelayedInteractable(CameraDelayTime);
		FarmField = nullptr;
	}
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


void UCFarmFieldWidget::OnNutritionStateChanged_Implementation(ENutritionState InState)
{
	switch (InState)
	{
		case ENutritionState::Famine: 
		{
			NutritionState->SetStateIconColor(FLinearColor::Red);
		}
		break;
		case ENutritionState::Enough:
		{
			NutritionState->SetStateIconColor(FLinearColor::Yellow);
		}
		break;
		case ENutritionState::Over:
		{
			NutritionState->SetStateIconColor(FLinearColor::Red);
		}
		break;
	default:
		break;
	}
}

void UCFarmFieldWidget::OnMoistureStateChanged_Implementation(EMoistureState InState)
{
	switch (InState)
	{
		case EMoistureState::Dry:
		{
			MoistureState->SetStateIconColor(FLinearColor::Red);
		}
		break;
		case EMoistureState::Enough:
		{
			MoistureState->SetStateIconColor(FLinearColor::Blue);
		}
		break;
		case EMoistureState::Humid:
		{
			MoistureState->SetStateIconColor(FLinearColor::Red);
		}
		break;
	default:
		break;
	}
}

void UCFarmFieldWidget::CheckPlantBtnActive(int32 InIndex)
{
	if (IsValid(FarmField->GetCrop()))
	{
		PlantBtn->SetIsEnabled(false);
		return;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AHUD* HUD=PC->GetHUD())
		{
			if (ACHUD* MyHud = Cast<ACHUD>(HUD))
			{
				if (UCHUDWidget* HudWidget = MyHud->GetHUD())
				{
					UE_LOG(LogTemp, Warning, TEXT("ConsumalbeType : %s"), *UEnum::GetValueAsString(HudWidget->GetQuickSlotBar()->GetCurrentSlotData().ConsumableType));
					UE_LOG(LogTemp, Warning, TEXT("ConsumableType Enum Value: %d"), static_cast<int32>(HudWidget->GetQuickSlotBar()->GetCurrentSlotData().ConsumableType));
					UE_LOG(LogTemp, Warning, TEXT("Seed Enum Value: %d"), static_cast<int32>(EConsumableType::Seed));
					if (HudWidget->GetQuickSlotBar()->GetCurrentSlotData().ConsumableType == EConsumableType::Seed)
					{
						PlantBtn->SetVisibility(ESlateVisibility::Visible);
						PlantBtn->SetIsEnabled(true);
						PlantBtn->SetRenderOpacity(1.0f);
						UE_LOG(LogTemp, Error, TEXT("Plant Btn Enabled"));
						return;
					}
				}
			}
		}
	}

	PlantBtn->SetVisibility(ESlateVisibility::Hidden);
	PlantBtn->SetIsEnabled(false);
	UE_LOG(LogTemp, Error, TEXT("Plant Btn Disabled"));
}

void UCFarmFieldWidget::OnPlantClicked()
{
	if (IsValid(FarmField))
	{
		FarmField->SetIsPlanting();
	}
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ACPlayer* Player = Cast<ACPlayer>(PC->GetPawn()))
		{
			Player->StartPlantingAnimation();
		}
	}
}
