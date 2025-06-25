#include "Item/CItem_WateringCan.h"
#include "Farm/CFarmField.h"
#include "Components/CInteractComponent.h"
#include "Components/CMoistureComponent.h"
#include "Characters/CPlayer.h"

bool ACItem_WateringCan::UseItem()
{
	Super::UseItem();
	if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
	{
		if (UCInteractComponent* InteractComp = Player->GetInteractComponent())
		{
			if (ACFarmField* Field = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
			{
				Player->StartWateringAnimation();
				UseEffect->Activate();
				Field->GetMoistureComp()->AddMoisture(FMath::Min(CurrentAmount, UseAmount));
				if (CurrentAmount < UseAmount)
				{
					CurrentAmount = 0.0f;
				}
				else
				{
					CurrentAmount -= UseAmount;
				}
				return true;
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
