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
	CurrentSlotCnt = 0;
	DefaultSlotCnt = 20;
	MaxSlotCnt = 50;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateInventoryWidget();
}

bool UCInventoryComponent::AddItem(ACItemBase* InItemActor)
{
	UGameInstance* Instance = GetOwner()->GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{
			TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(InItemActor->GetItemID());
			TOptional<FItemAssetData> ItemAssetDataOpt = MyInstance->GetItemtAssetData(InItemActor->GetItemID());
			if (ItemDataOpt.IsSet() && ItemAssetDataOpt.IsSet())
			{
				TArray<int32> ChangedIndexes;
				int32 PrevCount = InItemActor->GetAvailableCount();
				// Get Item Data
				const FItemData& ItemData = ItemDataOpt.GetValue();
				const FItemAssetData& ItemAssetData = ItemAssetDataOpt.GetValue();

				float TotalWeight = ItemData.ItemWeight * InItemActor->GetAvailableCount();
				int32 MaxPossibleCount = FMath::FloorToInt((MaxCapacity - CurrentCapacity) / ItemData.ItemWeight);

				UE_LOG(LogItem, Error, TEXT("Start add item."));
				if (MaxPossibleCount <= 0)
				{
					// 경고 위젯 출력 ("Inventory capacity is full!")
					ShowWarningWidget("Inventory capacity is full!");
					UE_LOG(LogItem, Error, TEXT("Inventory capacity is full! Curernt/Max Capacity : %f / %f,"),CurrentCapacity,MaxCapacity);
					return false;
				}

				if (!AddToExistingSlot(InItemActor,ChangedIndexes))
				{
					AddToNewSlot(InItemActor, ItemData, ItemAssetData, ChangedIndexes);
					if (ChangedIndexes.Num() == 0)
					{
						UE_LOG(LogItem, Error, TEXT("Inventory slots are full!"));
						return false;
					}
				}
				// Update Inventory Widget
				OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);

				// Show Item Notification
				int32 AddedItemCount = PrevCount - InItemActor->GetAvailableCount();
				UTexture2D* ItemIcon;
				CHelpers::GetAssetDynamic(&ItemIcon, ItemAssetData.ItemIconTextureRef);
				FName ItemName = InItemActor->GetInteractName();
				OnItemAdded.Broadcast(ItemName, AddedItemCount, ItemIcon);

				// Increase InventoryCapacity
				CurrentCapacity += AddedItemCount * ItemData.ItemWeight;
				// If get all Item, item actor must be destroyed.
				if (InItemActor->GetAvailableCount() <= 0)
				{
					InItemActor->Destroy();
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
		if (!CreateInventoryWidget())
		{
			UE_LOG(LogItem, Error, TEXT("Can't craete InventoryWidget."));
			return;
		}
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

bool UCInventoryComponent::CreateInventoryWidget()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		InventoryWidget = CreateWidget<UCInventoryWidget>(PlayerController, InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->SetInventoryComp(this);
			OnInventorySlotDataUpdated.AddDynamic(InventoryWidget, &UCInventoryWidget::UpdateInventorySlotWidget);
			OnInventorySlotCountUpdated.AddDynamic(InventoryWidget, &UCInventoryWidget::UpdateInventorySlotCount);
			OnMoneyUpdated.AddDynamic(InventoryWidget, &UCInventoryWidget::UpdateMoneyAmount);

			IncreaseSlotCount(DefaultSlotCnt);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			return true;
		}
	}
	return false;
}

bool UCInventoryComponent::AddToExistingSlot(ACItemBase* InItemActor, TArray<int32>& ChangedIndexes)
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		FInventorySlot& Slot = InventorySlots[i];
		if (Slot.ItemID == InItemActor->GetItemID() && Slot.CurrentStack < Slot.MaxStackSize)
		{
			int32 AvailableSpace = Slot.MaxStackSize - Slot.CurrentStack;
			int32 AmountToAdd = FMath::Min(InItemActor->GetAvailableCount(), AvailableSpace);

			Slot.CurrentStack += AmountToAdd;
			InItemActor->ReduceAvailableCount(AmountToAdd);
			ChangedIndexes.AddUnique(i);
			if (InItemActor->GetAvailableCount() <= 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool UCInventoryComponent::AddToNewSlot(ACItemBase* InItemActor, const FItemData& InItemData, const FItemAssetData& InItemAssetData, TArray<int32>& ChangedIndexes)
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		FInventorySlot& Slot = InventorySlots[i];
		if (Slot.ItemID == EItemID::None)
		{
			int32 AmountToAdd = FMath::Min(InItemActor->GetAvailableCount(), (int32)(InItemData.MaxStackSize));
			Slot.ItemID = InItemData.ItemID;
			Slot.CurrentStack = AmountToAdd;
			Slot.MaxStackSize = InItemData.MaxStackSize;
			Slot.MaxDurability = InItemData.MaxDurability;
			Slot.CurrentDurability = InItemActor->GetCurrentDruability();

			InItemActor->ReduceAvailableCount(AmountToAdd);
			ChangedIndexes.AddUnique(i);

			if ((InItemAssetData.ItemIconTextureRef).IsEmpty())
			{
				UE_LOG(LogItem, Error, TEXT("ItemIconTexture Reference is missing. ItemID : "), *(UEnum::GetValueAsString(Slot.ItemID)));
			}
			else
			{
				CHelpers::GetAssetDynamic(&Slot.ItemIcon, InItemAssetData.ItemIconTextureRef);
			}
			Slot.Description = InItemAssetData.Description;
		}
		else
		{
			UE_LOG(LogItem, Error, TEXT("ItemID : "), *(UEnum::GetValueAsString(Slot.ItemID)));
		}
		if (InItemActor->GetAvailableCount() <= 0)
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
			MyGameInstance->ShowWarningWidget(Message);
		}
	}
}

void UCInventoryComponent::SwapSlot(int32& SlotIndex1, int32& SlotIndex2)
{
	UE_LOG(LogItem, Log, TEXT("InventorySlots.Num() : %d"), InventorySlots.Num());
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

bool UCInventoryComponent::AddMoney(int32 InGetAmount)
{
	if (InGetAmount > 0)
	{
		OwnMoney += InGetAmount;
		OnMoneyUpdated.Broadcast(OwnMoney);
		return true;
	}
	
	UE_LOG(LogItem, Error, TEXT("UCInventoryComponent::AddMoney InValid Value : %d"), InGetAmount);
	return false;
}

bool UCInventoryComponent::UseMoney(int32 InUsedAmount)
{
	if (InUsedAmount < OwnMoney)
	{
		OwnMoney -= InUsedAmount;
		OnMoneyUpdated.Broadcast(OwnMoney);
		return true;
	}

	UE_LOG(LogItem, Error, TEXT("UCInventoryComponent::UseMoney Too Much Value : %d"), InUsedAmount);
	return false;
}

