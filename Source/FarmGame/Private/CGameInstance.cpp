#include "CGameInstance.h"
#include "Crops\/CBase_Crop.h"

UCGameInstance::UCGameInstance()
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
}

void UCGameInstance::Init()
{
	
}

const TOptional<FCropData> UCGameInstance::GetCropDefaultData(FName InCropName)
{
	static TMap<FName, FCropData> CropDefaultDataMap;

	// Exception handling
	if (!CropDefaultTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Crop Default Data Table is missing. Crop: %s"), *InCropName.ToString());
		return {};
	}

	if (CropDefaultDataMap.Contains(InCropName))
	{
		return CropDefaultDataMap[InCropName];
	}

	FCropData* NewData = CropDefaultTable->FindRow<FCropData>(InCropName, "Lookup CropDefaultData");
	if (NewData)
	{
		CropDefaultDataMap.Add(InCropName, *NewData);
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
	static TMap<FName, TArray<FCropGrowthData>> CropGrowthDataMap;

	// Exception handling
	if (!CropGrowthTable.IsValid() || !CropDefaultTable.IsValid())
	{
		if (!CropGrowthTable.IsValid() && !CropDefaultTable.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Data Table are both missing. Crop: %s"), *InCropName.ToString());
		}
		else if (!CropGrowthTable.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Crop Growth Data Table are both missing. Crop: %s"), *InCropName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Crop Default Data Table is missing. Crop: %s"), *InCropName.ToString());
		}
		return {};
	}

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
		return CropGrowthDataMap[InCropName][InLevel - 1];
	}

	TArray<FCropGrowthData*> Rows;
	CropGrowthTable->GetAllRows(TEXT("Fetching all rows"), Rows);

	TArray<FCropGrowthData> NewRows;
	for (const auto& Data : Rows)
	{
		if (Data->CropName == InCropName)
		{
			NewRows.Add(*Data);
		}
	}

	if (NewRows.Num() > 0)
	{
		CropGrowthDataMap.Add(InCropName, NewRows);
		return CropGrowthDataMap[InCropName][InLevel - 1];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InCropName.ToString());
		return {};
	}
}

const TOptional<FItemData> UCGameInstance::GetItemtData(FName InItemName)
{
	static TMap<FName, FItemData> ItemDataMap;

	// Exception handling
	if (!ItemDataTable.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Itemt Data Table is missing. Item: %s"), *InItemName.ToString());
		return {};
	}

	if (ItemDataMap.Contains(InItemName))
	{
		return ItemDataMap[InItemName];
	}

	FItemData* NewData = ItemDataTable->FindRow<FItemData>(InItemName, "Lookup CropDefaultData");
	if (NewData)
	{
		ItemDataMap.Add(InItemName, *NewData);
		return ItemDataMap[InItemName];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Data about %s"), *InItemName.ToString());
		return {};
	}
}