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
				UseEffect->Activate();
				if (UCMoistureComponent* MoistureComp = Field->GetMoistureComp())
				{
					MoistureComp->AddMoisture(FMath::Min(CurrentAmount, UseAmount));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UseItem Error, WateringCan. MoistureComp is missing!"));
				}
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

void ACItem_WateringCan::PlayAnimation()
{
	if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
	{
		Player->StartWateringAnimation();
	}
}
