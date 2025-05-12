#pragma once

#include "CoreMinimal.h"
#include "Item/CItem_Consumable.h"
#include "CItem_Seed.generated.h"

UCLASS()
class FARMGAME_API ACItem_Seed : public ACItem_Consumable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Crop")
	TSubclassOf<ACBase_Crop> CropClass;

	UPROPERTY(EditDefaultsOnly,Category="Crop")
	FVector PlantLocation;
};
