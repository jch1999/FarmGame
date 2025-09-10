#include "Item/CItem_WateringCan.h"
#include "Farm/CFarmField.h"
#include "Crops/CBase_Crop.h"
#include "Components/CInteractComponent.h"
#include "Components/CMoistureComponent.h"
#include "Characters/CPlayer.h"

bool ACItem_WateringCan::UseItem()
{
	if (Super::UseItem())
	{
		if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
		{
			if (UCInteractComponent* InteractComp = Player->GetInteractComponent())
			{
				if (ACFarmField* Field = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
				{
					UseEffect->Activate();
					UCMoistureComponent* MoistureComp_Field = Field->GetMoistureComp();
					UCMoistureComponent* MoistureComp_Crop = nullptr;
					if (IsValid(Field->GetCrop()))
					{
						MoistureComp_Crop = Field->GetCrop()->GetMoistureComp();

					}

					if (IsValid(MoistureComp_Field) && IsValid(MoistureComp_Crop))
					{
						MoistureComp_Field->AddMoisture(UseAmount * 0.75f);
						MoistureComp_Crop->AddMoisture(UseAmount * 0.25f);
					}
					else if (IsValid(MoistureComp_Field))
					{
						MoistureComp_Field->AddMoisture(UseAmount);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UseItem Error, WateringCan. MoistureComp is missing!"));
						return false;
					}

					return true;
				}
			}
		}
	}
	return false;
}

void ACItem_WateringCan::EndUse()
{
	Super::EndUse();
	UseEffect->Deactivate();
}

void ACItem_WateringCan::PlayAnimation()
{
	if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
	{
		Player->StartWateringAnimation();
	}
}
