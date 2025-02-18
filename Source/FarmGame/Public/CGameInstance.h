#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/CItemInterface.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Crops/CBase_Crop.h"
#include "CGameInstance.generated.h"

UCLASS()
class FARMGAME_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCGameInstance();

protected:
	virtual void Init() override;

public:
	// Get Crop Data
	const TOptional<FCropData> GetCropDefaultData(FName InCropName);

	const TOptional<FCropGrowthData> GetCropGrowthData(FName InCropName, int32 InLevel);

	// Get Item Data
	UFUNCTION(BlueprintCallable)
	const TOptional<FItemData> GetItemtData(FName InItemName);

public:
	// Crop Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Crop")
	TSoftObjectPtr<UDataTable> CropDefaultTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Crop")
	TSoftObjectPtr<UDataTable> CropGrowthTable;

	// Item Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Item")
	TSoftObjectPtr<UDataTable> ItemDataTable;
};
