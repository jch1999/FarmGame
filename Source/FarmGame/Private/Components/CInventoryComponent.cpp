#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CGameInstance.h"

UCInventoryComponent::UCInventoryComponent()
{
	NowCapacity = 0.0f;
	MaxCapacity = 100.0f;
	MaxSlotCnt = 20;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCInventoryComponent::AddItem(FItemData& InItemData, uint8& InCount)
{
	float TotalWeight = InItemData.ItemWeight * InCount;
	uint8 MaxPossibleCount = FMath::FloorToInt((MaxCapacity - NowCapacity)) / InItemData.ItemWeight);
	InCount -= MaxPossibleCount;
	if (MaxPossibleCount <= 0)
	{
		// 경고 위젯 출력 ("Inventory capacity is full!")
		ShowWarningWidget("Inventory capacity is full!");
		return false;
	}

	if (!AddToExistingSlot(InItemData, MaxPossibleCount))
	{
		AddToNewSlot(InItemData, MaxPossibleCount);
	}
	InCount += MaxPossibleCount;
	return true;
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
	while (InCount > 0)
	{
		if (InventorySlots.Num() >= MaxSlotCnt)
		{
			ShowWarningWidget("No empty slot available!");
			return false;
		}

		uint8 AmountToAdd = FMath::Min((uint8)InCount, InItemData.MaxStackSize);
		InventorySlots.Add(FInventorySlot{ InItemData.ItemID, AmountToAdd, InItemData.MaxStackSize });
		InCount -= AmountToAdd;
	}
	return true;
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
