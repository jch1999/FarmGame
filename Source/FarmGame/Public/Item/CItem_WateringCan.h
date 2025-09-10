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
	virtual void EndUse() override;
	virtual void PlayAnimation() override;

};
