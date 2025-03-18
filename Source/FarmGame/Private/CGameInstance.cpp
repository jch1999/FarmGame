#include "CGameInstance.h"
#include "Crops\/CBase_Crop.h"
#include "Blueprint/UserWidget.h"
#include "UI/CWarningWidget.h"
#include "UI/CDragIconWidget.h"
#include "UI/CFarmFieldWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Global.h"
#include "Controller\/CPlayerController.h"

UCGameInstance::UCGameInstance()
{
	if (!WarningWidgetClass)
	{
		CHelpers::GetClass(&WarningWidgetClass, "/Game/UI/WB_WarningWidget");
	}
	if (!FarmFieldWidgetClass)
	{
		CHelpers::GetClass<UCFarmFieldWidget>(&FarmFieldWidgetClass, "/Game/UI/WB_FarmFieldWidget");
	}
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

	//Interact
	if (!InteractAssetDataTable.IsValid())
	{
		InteractAssetDataTable = InteractAssetDataTable.LoadSynchronous();
	}

	LoadCropDefaultTable();
	LoadCropGrowthTable();
	LoadItemDataTable();
	LoadItemAssetDataTable();
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

void UCGameInstance::StartDragging(UTexture2D* ItemIcon)
{
	if (!DragIconWidget)
	{
		DragIconWidget = CreateWidget<UCDragIconWidget>(GetWorld(), DragIconWidgetClass);
		if (DragIconWidget)
		{
			DragIconWidget->SetDesiredSizeInViewport(FVector2D(30, 30));
			DragIconWidget->AddToViewport();
		}
	}
	DragIconWidget->InitDragIcon(ItemIcon);
	DragIconWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	OnDragIconShowing.Broadcast(true);
}

void UCGameInstance::StopDragging()
{
	if (DragIconWidget)
	{
		DragIconWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	OnDragIconShowing.Broadcast(false);
}

void UCGameInstance::UpdateDragIconPosition(FVector2D NewPosition)
{
	if (DragIconWidget)
	{
		DragIconWidget->SetRenderTranslation(NewPosition);
	}
}

void UCGameInstance::ShowWarningWidget(FString Message)
{
	if (!WarningWidget)
	{
		WarningWidget = CreateWidget<UCWarningWidget>(GetWorld(), WarningWidgetClass);
		WarningWidget->SetDesiredSizeInViewport(FVector2D(450, 150));
		WarningWidget->AddToViewport();
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
		if (MyPC)
		{
			MyPC->ShowWidget(WarningWidget);
		}
	}

	WarningWidget->SetWarningText(Message);
}


void UCGameInstance::HideWarningWidget()
{
	if (WarningWidget)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
			if (MyPC)
			{
				MyPC->HideWidget(WarningWidget);
			}
		}
	}
}

void UCGameInstance::ShowFarmFieldWidget(ACFarmField* TargetField)
{
	if (FarmFieldWidgetClass && !FarmFieldWidget)
	{
		FarmFieldWidget = CreateWidget<UCFarmFieldWidget>(GetWorld(), FarmFieldWidgetClass);
		if (FarmFieldWidget)
		{
			FarmFieldWidget->AddToViewport();
		}
	}

	if (FarmFieldWidget)
	{
		FarmFieldWidget->SetFarmField_Implementation(TargetField);
		FarmFieldWidget->GetPlantBtn()->OnClicked.AddDynamic(this, &UCGameInstance::HideFarmFieldWidget);
		FarmFieldWidget->PositionStateDisplays();

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
			if (MyPC)
			{
				MyPC->ShowWidget(FarmFieldWidget);
			}
		}
	}
}

void UCGameInstance::HideFarmFieldWidget()
{
	if (FarmFieldWidget)
	{
		FarmFieldWidget->ResetFarmField_Implementation();
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
			if (MyPC)
			{
				MyPC->HideWidget(FarmFieldWidget);
			}
		}
	}
}