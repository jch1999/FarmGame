#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "CItem_Consumable.generated.h"

UCLASS()
class FARMGAME_API ACItem_Consumable : public ACItemBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category ="Consumable")
	EConsumableType ConsumableType;
};
