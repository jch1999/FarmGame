#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "Crops/CBase_Crop.h"
#include "CItem_Crop.generated.h"

UCLASS()
class FARMGAME_API ACItem_Crop : public ACItemBase
{
	GENERATED_BODY()
	
public:
	ACItem_Crop();

public:
	UFUNCTION(BlueprintCallable,Category="Crop")
	bool SetCropData(FCropData& InData);

	// From Interface
	virtual bool UseItem() override;;
	virtual void Interact(AActor* OtherActor) override;

public:
	UPROPERTY(EditAnywhere, Category="Crop|Data")
	FCropData CropData;
};
