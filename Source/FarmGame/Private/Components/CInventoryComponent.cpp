#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CGameInstance.h"
#include "Global.h"
#include "UI/CInventoryWidget.h"
#include "UI/CQuickSlotBarWidget.h"
#include "Item/CItemBase.h"

UCInventoryComponent::UCInventoryComponent()
{
	CHelpers::GetClass(&InventoryWidgetClass, "/Game/UI/WB_CInventoryWidget");
	CHelpers::GetClass(&QuickSlotBarWidgetClass, "/Game/UI/WB_CQuickSlotBar");
	CurrentCapacity = 0.0f;
	MaxCapacity = 100.0f;
	CurrentSlotCnt = 20;
	MaxSlotCnt = 50;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCInventoryComponent::AddItem(ACItemBase* InItemActor, int32& InCount)
{
	UGameInstance* Instance = GetOwner()->GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(InItemActor->GetItemID());
			if (ItemDataOpt.IsSet())
			{
				FItemData ItemData = ItemDataOpt.GetValue();
				float TotalWeight = ItemData.ItemWeight * InCount;
				int32 MaxPossibleCount = FMath::FloorToInt((MaxCapacity - CurrentCapacity) / ItemData.ItemWeight);

				UE_LOG(LogItem, Error, TEXT("Start add item."));
				if (MaxPossibleCount <= 0)
				{
					// 경고 위젯 출력 ("Inventory capacity is full!")
					ShowWarningWidget("Inventory capacity is full!");
					UE_LOG(LogItem, Error, TEXT("Inventory capacity is full!"));
					return false;
				}
				uint8 AmountToAdd = FMath::Min(InCount, MaxPossibleCount);
				InCount -= AmountToAdd;
				if (!AddToExistingSlot(ItemData, AmountToAdd))
				{
					AddToNewSlot(ItemData, AmountToAdd);
				}
				InCount += AmountToAdd;

				TOptional<FItemAssetData> ItemAssetDataOpt = MyInstance->GetItemtAssetData(InItemActor->GetItemID());
				if (ItemAssetDataOpt.IsSet())
				{
					FItemAssetData& ItemAssetData = ItemAssetDataOpt.GetValue();
					UTexture2D* ItemIcon;
					CHelpers::GetAssetDynamic(&ItemIcon, ItemAssetData.ItemIconTextureRef);
					FName ItemName = InItemActor->GetInteractName();
					OnItemAdded.Broadcast(ItemName,MaxPossibleCount,ItemIcon);
				}
				if (InCount == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void UCInventoryComponent::ShowInventory()
{
	if (InventoryWidget == nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController) return;

		InventoryWidget = CreateWidget<UCInventoryWidget>(PlayerController, InventoryWidgetClass);
		if (!InventoryWidget)
		{
			UE_LOG(LogItem, Error, TEXT("Can't craete InventoryWidget."));
			return;
		}
		InventoryWidget->SetInventoryComp(this);
		OnInventorySlotDataUpdated.AddDynamic(InventoryWidget, &UCInventoryWidget::UpdateInventorySlotWidget);
		OnInventorySlotCountUpdated.AddDynamic(InventoryWidget, &UCInventoryWidget::UpdateInventorySlotCount);

		IncreaseSlotCount(CurrentSlotCnt);
		InventoryWidget->AddToViewport();
	}
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogItem, Display, TEXT("Open Inventory."));
}

void UCInventoryComponent::HideInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden); 
		UE_LOG(LogItem, Display, TEXT("Close Inventory."));
	}
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
			Slot.MaxDurability = InItemData.MaxDurability;
			Slot.CurrentDurability=
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
						Slot.Description = AssetData.Description;
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

bool UCInventoryComponent::IncreaseSlotCount(int32 InSlotCount)
{
	if (CurrentSlotCnt >= MaxSlotCnt) return false;
	CurrentSlotCnt = FMath::Min(CurrentSlotCnt + InSlotCount, MaxSlotCnt);
	InventorySlots.SetNum(CurrentSlotCnt);
	OnInventorySlotCountUpdated.Broadcast(InSlotCount);
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

void UCInventoryComponent::SwapSlot(int32& SlotIndex1, int32& SlotIndex2)
{
	bool bCheck1 = (SlotIndex1 < 0 || SlotIndex1 >= InventorySlots.Num());
	bool bCheck2 = (SlotIndex2 < 0 || SlotIndex2 >= InventorySlots.Num());
	if (bCheck1 || bCheck2)
	{
		UE_LOG(LogItem, Error, TEXT("Invalid Index! InventoryComponent::SwapSlot(%d, %d)"), SlotIndex1, SlotIndex2);
		return;
	}
	if (SlotIndex1 == SlotIndex2)
	{
		UE_LOG(LogItem, Error, TEXT("Same Slot! No swap needed."));
		return;
	}

	FInventorySlot& Slot1 = InventorySlots[SlotIndex1];
	FInventorySlot& Slot2 = InventorySlots[SlotIndex2];
	FInventorySlot TempSlot = Slot1;
	Slot1 = Slot2;
	Slot2 = TempSlot;

	TArray<int32> ChangedIndexes;
	ChangedIndexes.Add(SlotIndex1);
	ChangedIndexes.Add(SlotIndex2);
	OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);
}

void UCInventoryComponent::UseItem(int32& SlotIndex)
{
}

void UCInventoryComponent::ClearSlot(int32 InIndex)
{
	if (InventorySlots.IsValidIndex(InIndex))
	{
		InventorySlots[InIndex] = FInventorySlot();
		TArray<int32> ChangedIndexes;
		ChangedIndexes.Add(InIndex);
		OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);
	}
}

TWeakPtr<FInventorySlot> UCInventoryComponent::GetSlotWeak(int32 Index)
{
	if (InventorySlots.IsValidIndex(Index))
    {
        return TWeakPtr<FInventorySlot>(MakeShared<FInventorySlot>(InventorySlots[Index]));
    }
    return TWeakPtr<FInventorySlot>();
}

