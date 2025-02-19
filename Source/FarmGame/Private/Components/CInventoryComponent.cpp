#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CGameInstance.h"

UCInventoryComponent::UCInventoryComponent()
{
	NowCapacity = 0.0f;
	MaxCapacity = 100.0f;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCInventoryComponent::AddItem(FItemData& InItemData, int32& InCount)
{
	float TotalWeight = InItemData.ItemWeight * InCount;
	if (NowCapacity + TotalWeight <= MaxCapacity)
	{
		if (InventoryItems.Contains(InItemData.ItemID))
		{
			InventoryItems[InItemData.ItemID] += InCount;
		}
		else
		{
			InventoryItems.Add(InItemData.ItemID, InCount);
		}

		NowCapacity += TotalWeight;
		InCount = 0;
		return true;
	}
	else 
	{
		int32 MaxPossibleCount = FMath::FloorToInt((MaxCapacity - NowCapacity) / InItemData.ItemWeight);

		if (MaxPossibleCount > 0)
		{
			if (InventoryItems.Contains(InItemData.ItemID))
			{
				InventoryItems[InItemData.ItemID] += MaxPossibleCount;
			}
			else
			{
				InventoryItems.Add(InItemData.ItemID, MaxPossibleCount);
			}
			NowCapacity += MaxPossibleCount * InItemData.ItemWeight;
			InCount -= MaxPossibleCount;
			return false;
		}

		// 경고 위젯 출력 ("Inventory capacity is full!")
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
