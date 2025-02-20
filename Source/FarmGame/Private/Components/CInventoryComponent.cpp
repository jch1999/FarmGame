#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CGameInstance.h"
#include "Global.h"

UCInventoryComponent::UCInventoryComponent()
{
	CurrentCapacity = 0.0f;
	MaxCapacity = 100.0f;
	CurrentSlotCnt = 20;
	MaxSlotCnt = 50;
	InventorySlots.SetNum(CurrentSlotCnt);
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCInventoryComponent::AddItem(FItemData& InItemData, int32& InCount)
{
	float TotalWeight = InItemData.ItemWeight * InCount;
	int32 MaxPossibleCount = FMath::FloorToInt((MaxCapacity - CurrentCapacity) / InItemData.ItemWeight);
	
	if (MaxPossibleCount <= 0)
	{
		// 경고 위젯 출력 ("Inventory capacity is full!")
		ShowWarningWidget("Inventory capacity is full!");
		return false;
	}
	uint8 AmountToAdd = FMath::Min(InCount, MaxPossibleCount);
	InCount -= AmountToAdd;
	if (!AddToExistingSlot(InItemData, AmountToAdd))
	{
		AddToNewSlot(InItemData, AmountToAdd);
	}
	InCount += AmountToAdd;
	
	if (InCount == 0)
	{
		return true;
	}

	return false;
}

bool UCInventoryComponent::AddToExistingSlot(FItemData& InItemData, uint8& InCount)
{
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemID == InItemData.ItemID && Slot.CurrentStack < Slot.MaxStackSize)
		{
			uint8 AvailableSpace = Slot.MaxStackSize - Slot.CurrentStack;
			uint8 AmountToAdd = FMath::Min(InCount, AvailableSpace);

			Slot.CurrentStack += AmountToAdd;
			InCount -= AmountToAdd;

			if (InCount <= 0) return true;
		}
	}
	return false;
}

bool UCInventoryComponent::AddToNewSlot(FItemData& InItemData, uint8& InCount)
{
	if (CurrentSlotCnt >= MaxSlotCnt)
	{
		ShowWarningWidget("No empty slot available!");
		return false;
	}
	for (auto& Slot : InventorySlots)
	{
		if (Slot.ItemID == EItemID::None)
		{
			int32 AmountToAdd = FMath::Min(InCount, InItemData.MaxStackSize);
			Slot.ItemID = InItemData.ItemID;
			Slot.CurrentStack = AmountToAdd;
			Slot.MaxStackSize = InItemData.MaxStackSize;
			
			if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
			{
				if (UCGameInstance* MyGameInstance = Cast<UCGameInstance>(GameInstance))
				{
					TOptional<FItemAssetData> ItemAssetDataOpt = MyGameInstance->GetItemtAssetData(InItemData.ItemID);
					if (ItemAssetDataOpt.IsSet())
					{
						FItemAssetData& AssetData= ItemAssetDataOpt.GetValue();
						if ((AssetData.ItemIconTextureRef).IsEmpty())
						{
							UE_LOG(LogItem, Error, TEXT("ItemIconTexture Reference is missing. ItemID : "),*(UEnum::GetValueAsString(Slot.ItemID)));
						}
						else
						{
							CHelpers::GetAssetDynamic(&Slot.ItemIcon, AssetData.ItemIconTextureRef);
						}
					}
				}
			}
		}

		if (InCount <= 0)
		{
			return true;
		}
	}
	
	return false;
}

void UCInventoryComponent::ShowWarningWidget(FString Message)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		if (UCGameInstance* MyGameInstance = Cast<UCGameInstance>(GameInstance))
		{
			MyGameInstance->ShowWarningWidget("Inventory is full!");
		}
	}
}
