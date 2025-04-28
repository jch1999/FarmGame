#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "CItem_Consumable.generated.h"

UCLASS()
class FARMGAME_API ACItem_Consumable : public ACItemBase
{
	GENERATED_BODY()

public:
	// From Interface
	virtual bool UseItem() override;;
	//virtual void Interact(AActor* OtherActor) override;

public:
	UPROPERTY(EditDefaultsOnly, Category ="Consumable")
	EConsumableType ConsumableType;
};
