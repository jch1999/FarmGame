#pragma once

#include "CoreMinimal.h"
#include "Item/CItem_Tool.h"
#include "CItem_WateringCan.generated.h"

UCLASS()
class FARMGAME_API ACItem_WateringCan : public ACItem_Tool
{
	GENERATED_BODY()
	
public:
	virtual bool UseItem() override;

public:
	UPROPERTY(EditAnywhere,Category="Watering")
	float CurrentAmount;

	UPROPERTY(EditAnywhere, Category = "Watering")
	float UseAmount;

	UPROPERTY(EditAnywhere, Category = "Watering")
	float MaxAmount;

};
