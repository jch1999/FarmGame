#pragma once

#include "CoreMinimal.h"
#include "Item/CItem_Tool.h"
#include "CItem_Hoe.generated.h"

UCLASS()
class FARMGAME_API ACItem_Hoe : public ACItem_Tool
{
	GENERATED_BODY()
	
public:
	virtual bool UseItem_Implementation() override;
	virtual void EndUse_Implementation() override;
	virtual bool PlayAnimation_Implementation() override;
};
