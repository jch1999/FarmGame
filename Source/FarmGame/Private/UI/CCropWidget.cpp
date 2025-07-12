#include "UI/CCropWidget.h"
#include "Crops/CBase_Crop.h"
#include "Components/CNutritionComponent.h"
#include "Components/CMoistureComponent.h"
#include "Components/CHealthComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/CStateDisplayWidget.h"
#include "Global.h"

void UCCropWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
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

	if (HealthState)
	{
		UTexture2D* HealthIcon;
		CHelpers::GetAssetDynamic(&HealthIcon, "/Game/ThirdParty/Icon/Icon_Health");
		HealthState->SetStateIconTexture(HealthIcon);
		HealthState->SetStateIconColor(FLinearColor(150.0f, 75.0f, 0.0f));
	}

	if (GrowthState)
	{
		UTexture2D* GrowthIcon;
		CHelpers::GetAssetDynamic(&GrowthIcon, "/Game/ThirdParty/Icon/Icon_Growth");
		GrowthState->SetStateIconTexture(GrowthIcon);
		GrowthState->SetStateIconColor(FLinearColor(150.0f, 75.0f, 0.0f));
	}
}

void UCCropWidget::SetCrop_Implementation(ACBase_Crop* InCrop)
{
	Crop = InCrop;

	UCNutritionComponent* NutritionComp = Crop->GetNutritionComp();
	if (NutritionComp)
	{
		NutritionComp->OnStateValueChanged.AddDynamic(this, &UCCropWidget::UpdateNutrition);
		NutritionComp->OnNutritionStateChanged.AddDynamic(this, &UCCropWidget::OnNutritionStateChanged);
		UpdateNutrition(0.0f, NutritionComp->GetCurrentValue(), NutritionComp->GetMaxValue());
		OnNutritionStateChanged(NutritionComp->GetCurrentState());
	}

	UCMoistureComponent* MoistureComp = Crop->GetMoistureComp();
	if (MoistureComp)
	{
		MoistureComp->OnStateValueChanged.AddDynamic(this, &UCCropWidget::UpdateMoisture); 
		MoistureComp->OnMoistureStateChanged.AddDynamic(this, &UCCropWidget::OnMoistureStateChanged);
		UpdateMoisture(0.0f, MoistureComp->GetCurrentValue(), MoistureComp->GetMaxValue());
		OnMoistureStateChanged(MoistureComp->GetCurrentState());
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
		UCNutritionComponent* NutritionComp = Crop->GetNutritionComp();
		if (NutritionComp)
		{
			NutritionComp->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateNutrition);
			NutritionComp->OnNutritionStateChanged.RemoveDynamic(this, &UCCropWidget::OnNutritionStateChanged);
		}
		
		UCMoistureComponent* MoistureComp = Crop->GetMoistureComp();
		if (MoistureComp)
		{
			Crop->GetMoistureComp()->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateMoisture);
			Crop->GetMoistureComp()->OnMoistureStateChanged.RemoveDynamic(this, &UCCropWidget::OnMoistureStateChanged);
		}

		UCHealthComponent* HealthComp = Crop->GetHealthComp();
		if (HealthComp)
		{
			Crop->GetHealthComp()->OnStateValueChanged.RemoveDynamic(this, &UCCropWidget::UpdateHealth);
		}

		if (Crop)
		{
			Crop->OnGrowthChanged.RemoveDynamic(this, &UCCropWidget::UpdateGrowth);
		}
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

void UCCropWidget::OnNutritionStateChanged_Implementation(ENutritionState InState)
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

void UCCropWidget::OnMoistureStateChanged_Implementation(EMoistureState InState)
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
