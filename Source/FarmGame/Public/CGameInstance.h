#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/CItemInterface.h"
#include "Interfaces/CInterface_Interactable.h"
#include "Crops/CBase_Crop.h"
#include "CGameInstance.generated.h"

class UCWarningWidget;
class UCDragIconWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragIconShowing, bool, IsOn);


UCLASS()
class FARMGAME_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCGameInstance();

protected:
	virtual void Init() override;

public:
	void ShowWarningWidget(FString Message);
	UFUNCTION()
	void HideWarningWidget();

public:
	// Get Crop Data
	const TOptional<FCropData> GetCropDefaultData(FName InCropName);

	const TOptional<FCropGrowthData> GetCropGrowthData(FName InCropName, int32 InLevel);

	// Get Item Data
	const TOptional<FItemData> GetItemtData(EItemID InItemID);
	const TOptional<FItemAssetData> GetItemtAssetData(EItemID InItemID);

	// Get Interact Asset Data
	const TOptional<FInteractAssetData> GetInteractAssetData(EInteractObjectType InInteractType);
	
	void StartDragging(UTexture2D* ItemIcon);
	void StopDragging();
	void UpdateDragIconPosition(FVector2D NewPosition);

	UPROPERTY(BlueprintAssignable)
	FDragIconShowing OnDragIconShowing;


private:
	void LoadCropDefaultTable();
	void LoadCropGrowthTable();
	void LoadItemDataTable();
	void LoadItemAssetDataTable();
	void LoadInteractAssetDataTable();

public:
	// Crop Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Crop")
	TSoftObjectPtr<UDataTable> CropDefaultTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Crop")
	TSoftObjectPtr<UDataTable> CropGrowthTable;

	// Item Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Item")
	TSoftObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Item")
	TSoftObjectPtr<UDataTable> ItemAssetDataTable;

	// Interact Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable|Interact")
	TSoftObjectPtr<UDataTable> InteractAssetDataTable;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	TSubclassOf<UCWarningWidget> WarningWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	UCWarningWidget* WarningWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	TSubclassOf<UCDragIconWidget> DragIconWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|Warning")
	UCDragIconWidget* DragIconWidget;
	bool IsDragging;

private:
	// Data Map
	TMap<FName, FCropData> CropDefaultDataMap;
	TMap<FName, TArray<FCropGrowthData>*> CropGrowthDataMap;
	TMap<EItemID, FItemData> ItemDataMap;
	TMap<EItemID, FItemAssetData> ItemAssetDataMap;
	TMap<EInteractObjectType, FInteractAssetData> InteractAssetDataMap;
};
