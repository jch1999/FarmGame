#pragma once

#include "CoreMinimal.h"
#include "Item/CItem_Consumable.h"
#include "Interfaces/UsableItem.h"
#include "CItem_Fertilizer.generated.h"

UCLASS()
class FARMGAME_API ACItem_Fertilizer : public ACItem_Consumable, public IUsableItem
{
	GENERATED_BODY()

public:
	virtual bool UseItem_Implementation() override;
};
