#include "Item/CItem_Hoe.h"
#include "Farm/CFarmField.h"
#include "Crops/CBase_Crop.h"
#include "Components/CInteractComponent.h"
#include "Components/CCultivationComponent.h"
#include "Characters/CPlayer.h"
#include "Farm/CFarmField.h"

bool ACItem_Hoe::UseItem_Implementation()
{
	if (Super::UseItem_Implementation())
	{
		if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
		{
			if (UCInteractComponent* InteractComp = Player->GetInteractComponent())
			{
				if (ACFarmField* Field = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
				{
					if (IsValid(Field->GetCrop()))
					{
						UE_LOG(LogItem, Error, TEXT("Crops exist in the field. You can't cultivate the field."));
						return false;
					}
					UseEffect->Activate();

					UCCultivationComponent* CultivateComp = Field->GetCultivationComp();

					if (IsValid(CultivateComp))
					{
						CultivateComp->AddCultivation(UseAmount);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UseItem Error, Hoe. CultivationComp is missing!"));
						return false;
					}

					return true;
				}
			}
		}
	}
	return false;
}

void ACItem_Hoe::EndUse_Implementation()
{
	Super::EndUse_Implementation();
	UseEffect->Deactivate();
}

bool ACItem_Hoe::PlayAnimation_Implementation()
{
	Super::PlayAnimation_Implementation();
	if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
	{
		if(ACFarmField* Field = Cast<ACFarmField>(Player->GetInteractComponent()->GetActionInteractTarget()))
		{
			if (!Field->IsCultivable())
			{
				UE_LOG(LogItem, Error, TEXT("ACItem_Hoe::PlayAnimation_Implementation Error. The field is not cultivable."));
				return false;
			}
		}
		else
		{
			UE_LOG(LogItem, Error, TEXT("ACItem_Hoe::PlayAnimation_Implementation Error. FarmField Missing!"));
			return false;
		}
		Player->StartHoeingAnimation();
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("ACItem_Hoe::PlayAnimation_Implementation Error. Player Missing!"));
		return false;
	}
	return true;
}
