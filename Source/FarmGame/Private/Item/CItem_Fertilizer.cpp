#include "Item/CItem_Fertilizer.h"
#include "Crops/CBase_Crop.h"
#include "Components/CNutritionComponent.h"
#include "Components/CInteractcomponent.h"
#include "Farm/CFarmField.h"
#include "Characters/CPlayer.h"

bool ACItem_Fertilizer::UseItem_Implementation()
{
	UseItem_Common();
	if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
	{
		if (UCInteractComponent* InteractComp = Player->GetInteractComponent())
		{
			if (ACFarmField* Field = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
			{
				// UseEffect->Activate();
				UCNutritionComponent* NutritionComp_Field = Field->GetNutritionComp();
				UCNutritionComponent* NutritionComp_Crop = nullptr;
				if (IsValid(Field->GetCrop()))
				{
					NutritionComp_Crop = Field->GetCrop()->GetNutritionComp();

				}

				if (IsValid(NutritionComp_Field) && IsValid(NutritionComp_Crop))
				{
					NutritionComp_Field->AddNutrition(UseAmount * 0.75f);
					NutritionComp_Crop->AddNutrition(UseAmount * 0.25f);
				}
				else if (IsValid(NutritionComp_Field))
				{
					NutritionComp_Field->AddNutrition(UseAmount);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UseItem Error, Fertilizer. NutritionComp is missing!"));
					return false;
				}

				return true;
			}
		}
	}
	return false;
}
