#include "CGameInstance.h"
#include "Crops/CBase_Crop.h"
#include "CHUD.h"
#include "Global.h"

UCGameInstance::UCGameInstance()
{
	
}

void UCGameInstance::Init()
{
	// 데이터 테이블 강제 로드
	// Crop
	if (!CropDefaultTable.IsValid())
	{
		CropDefaultTable = CropDefaultTable.LoadSynchronous();
	}

	if (!CropGrowthTable.IsValid())
	{
		CropGrowthTable = CropGrowthTable.LoadSynchronous();
	}

	// Item
	if (!ItemDataTable.IsValid())
	{
		ItemDataTable = ItemDataTable.LoadSynchronous();
	}

	if (!ItemAssetDataTable.IsValid())
	{
		ItemAssetDataTable = ItemAssetDataTable.LoadSynchronous();
	}

	if (!ItemAttachDataTable.IsValid())
	{
		ItemAttachDataTable = ItemAttachDataTable.LoadSynchronous();
	}
	//Interact
	if (!InteractAssetDataTable.IsValid())
	{
		InteractAssetDataTable = InteractAssetDataTable.LoadSynchronous();
	}

	LoadCropDefaultTable();
	LoadCropGrowthTable();
	LoadItemDataTable();
	LoadItemAssetDataTable();
	LoadItemAttachDataTable();
	LoadInteractAssetDataTable();
}

void UCGameInstance::LoadCropDefaultTable()
{
	// Exception handling
	if (!CropDefaultTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Crop Default Data Table is missing."));
		return;
	}

	TArray<FName> RowNames = CropDefaultTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FCropData* Row = CropDefaultTable->FindRow<FCropData>(RowName, "LoadCropDefaultTable");
		if (Row)
		{
			CropDefaultDataMap.Add(RowName, *Row);
		}
	}
}

void UCGameInstance::LoadCropGrowthTable()
{// Exception handling
	if (!CropGrowthTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Crop Growth Data Table is missing."));
		return;
	}

	TArray<FName> RowNames = CropGrowthTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FCropGrowthData* Row = CropGrowthTable->FindRow<FCropGrowthData>(RowName, "LoadCropGrowthTable");
		if (Row)
		{
			if (!CropGrowthDataMap.Contains(Row->CropName))
			{
				CropGrowthDataMap.Add(Row->CropName, new TArray<FCropGrowthData>());
			}
			CropGrowthDataMap[Row->CropName]->Add(*Row);
		}
	}
}

void UCGameInstance::LoadItemDataTable()
{
	if (!ItemDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not valid!"));
		return;
	}

	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, "LoadItemDataTable");
		if (Row)
		{
			ItemDataMap.Add(Row->ItemID, *Row);
		}
	}
}

void UCGameInstance::LoadItemAssetDataTable()
{
	if (!ItemAssetDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemAssetDataTable is not valid!"));
		return;
	}

	TArray<FName> RowNames = ItemAssetDataTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemAssetData* Row = ItemAssetDataTable->FindRow<FItemAssetData>(RowName, "LoadItemAssetDataTable");
		if (Row)
		{
			ItemAssetDataMap.Add(Row->ItemID, *Row);
		}
	}
}

void UCGameInstance::LoadItemAttachDataTable()
{
	if (!ItemAssetDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemAttachDataTable is not valid!"));
		return;
	}

	TArray<FName> RowNames = ItemAttachDataTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemAttachData* Row = ItemAttachDataTable->FindRow<FItemAttachData>(RowName, "LoadItemAttachDataTable");
		if (Row)
		{
			ItemAttachDataMap.Add(Row->ItemID, *Row);
		}
	}
}

void UCGameInstance::LoadInteractAssetDataTable()
{
	if (!InteractAssetDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("InteractAssetDataTable is not valid!"));
		return;
	}

	TArray<FName> RowNames = InteractAssetDataTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FInteractAssetData* Row = InteractAssetDataTable->FindRow<FInteractAssetData>(RowName, "LoadItemAssetDataTable");
		if (Row)
		{
			InteractAssetDataMap.Add(Row->InteractType, *Row);
		}
	}
}

const TOptional<FCropData> UCGameInstance::GetCropDefaultData(FName InCropName)
{
	if (CropDefaultDataMap.Contains(InCropName))
	{
		return CropDefaultDataMap[InCropName];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InCropName.ToString());
		return {};
	}
}

const TOptional<FCropGrowthData> UCGameInstance::GetCropGrowthData(FName InCropName, int32 InLevel)
{
	
	TOptional<FCropData> CropDataOpt = GetCropDefaultData(InCropName);
	if (!CropDataOpt.IsSet())
	{
		return {};
	}

	const FCropData& CropData = CropDataOpt.GetValue();
	if (InLevel<1 || InLevel > CropData.MaxLevel)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Growth Level (%d) for Crop: %s"), InLevel, *InCropName.ToString());
		return {};
	}

	if (CropGrowthDataMap.Contains(InCropName))
	{
		 TArray<FCropGrowthData>* GrowthDataArray = CropGrowthDataMap[InCropName];
		 if (GrowthDataArray->IsValidIndex(InLevel - 1))
		 {
			 return (*GrowthDataArray)[InLevel - 1];
		 }
		 else
		 {
			 UE_LOG(LogTemp, Error, TEXT("Invalid Level (%d) for Crop: %s"), InLevel, *InCropName.ToString());
			 return {};
		 }
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InCropName.ToString());
		return {};
	}
}

const TOptional<FItemData> UCGameInstance::GetItemtData(EItemID InItemID)
{
	if (ItemDataMap.Contains(InItemID))
	{
		return ItemDataMap[InItemID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *(UEnum::GetValueAsString(InItemID)));
		return {};
	}
}

const TOptional<FItemAssetData> UCGameInstance::GetItemtAssetData(EItemID InItemID)
{
	UE_LOG(LogItem, Warning, TEXT("Requesting ItemAssetDAta for ID: %s"), *UEnum::GetValueAsString(InItemID));
	if (ItemAssetDataMap.Contains(InItemID))
	{
		return ItemAssetDataMap[InItemID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *(UEnum::GetValueAsString(InItemID)));
		return {};
	}

}

const TOptional<FItemAttachData> UCGameInstance::GetItemAttachData(EItemID InItemID)
{
	if (ItemAttachDataMap.Contains(InItemID))
	{
		return ItemAttachDataMap[InItemID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *(UEnum::GetValueAsString(InItemID)));
		return {};
	}
}

const TOptional<FInteractAssetData> UCGameInstance::GetInteractAssetData(EInteractObjectType InInteractType)
{
	if (InteractAssetDataMap.Contains(InInteractType))
	{
		return InteractAssetDataMap[InInteractType];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *(UEnum::GetValueAsString(InInteractType)));
		return {};
	}
}