#include "Item/CItem_WateringCan.h"
#include "Farm/CFarmField.h"
#include "Components/CInteractComponent.h"
#include "Components/CMoistureComponent.h"
#include "Characters/CPlayer.h"

bool ACItem_WateringCan::UseItem()
{
	if (UCInteractComponent* InteractComp= GetOwnerPlayer()->GetInteractComponent())
	{
		if (ACFarmField* Field = Cast<ACFarmField>(InteractComp->GetActionInteractTarget()))
		{
			Field->GetMoistureComp()->AddMoisture(FMath::Min(CurrentAmount,UseAmount));
			if (CurrentAmount < UseAmount)
			{
				CurrentAmount = 0.0f;
			}
			else
			{
				CurrentAmount -= UseAmount;
			}
			CurrentDurability--;
		}
	}
	return false;
}
