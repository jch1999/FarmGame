#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CGameInstance.h"
#include "Characters/CPlayer.h"
#include "Global.h"
#include "UI/CInventoryWidget.h"
#include "UI/CTitleBarWidget.h"
#include "Components/Button.h"
#include "Item/CItemBase.h"
#include "Item/CItem_Consumable.h"
#include "Controller/CPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractComponent.h"

UCInventoryComponent::UCInventoryComponent()
{
	CHelpers::GetClass(&InventoryWidgetClass, "/Game/UI/WB_CInventoryWidget");
	CurrentCapacity = 0.0f;
	MaxCapacity = 100.0f;
	CurrentSlotCnt = 0;
	DefaultSlotCnt = 20;
	MaxSlotCnt = 50;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateInventoryWidget();

	OwnerCharacter = Cast<ACPlayer>(GetOwner());

	if (AController* Controller = OwnerCharacter->GetController())
	{
		if (ACPlayerController* MyPC = Cast<ACPlayerController>(Controller))
		{
			MyPC->OnDragIconShowing.AddDynamic(this, &UCInventoryComponent::ToggleTick);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't get PlayerController"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get PlayerController"));
	}
}

void UCInventoryComponent::TickComponent(float DelatTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (AController* Controller = OwnerCharacter->GetController())
	{
		if (ACPlayerController* MyPC = Cast<ACPlayerController>(Controller))
		{
			MyPC->UpdateDragIconPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()));
		}
	}
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

bool UCInventoryComponent::ReduceItemStack(int InIndex)
{
	if (InventorySlots.IsValidIndex(InIndex))
	{
		// Reduce Capacity
		if (UGameInstance* Instance = GetOwner()->GetGameInstance())
		{
			if (UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance))
			{
				TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(InventorySlots[InIndex].ItemID);
				if (ItemDataOpt.IsSet())
				{
					const FItemData& ItemData = ItemDataOpt.GetValue();
					CurrentCapacity -= ItemData.ItemWeight;
				}
			}
		}

		--(InventorySlots[InIndex].CurrentStack);
		UE_LOG(LogTemp, Warning, TEXT("Left Stack : %d"), InventorySlots[InIndex].CurrentStack);
		if (InventorySlots[InIndex].CurrentStack <= 0)
		{
			UE_LOG(LogItem, Display, TEXT("Clear Slot!"));
			ClearSlot(InIndex);
		}
		else
		{
			TArray<int32> ChangedIndexes;
			ChangedIndexes.Add(InIndex);
			OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);
		}
		return true;
	}
	else
	{
		UE_LOG(LogItem, Error, TEXT("Invalid SlotIndex! Index : %d"), InIndex);
	}
	return false;
}

bool UCInventoryComponent::ReduceItemDurability(int InIndex, float ConsumedDurability)
{
	if (InventorySlots.IsValidIndex(InIndex))
	{
		if (InventorySlots[InIndex].CurrentDurability >= ConsumedDurability)
		{
			InventorySlots[InIndex].CurrentDurability -= ConsumedDurability;
			if (InventorySlots[InIndex].CurrentDurability > 0.0f)
			{
				TArray<int32> ChangedIndexes;
				ChangedIndexes.Add(InIndex);
				OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);
			}
			else
			{
				ClearSlot(InIndex);
			}
			return true;
		}
	}
	return false;
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
			if (UCTitleBarWidget* InventoryTitleBar = InventoryWidget->TitleBarWidget)
			{
				InventoryTitleBar->CloseButton->OnClicked.AddDynamic(this, &UCInventoryComponent::HideInventoryWidget);
			}

			IncreaseSlotCount(DefaultSlotCnt);
			// InventoryWidget->AddToViewport();
			// InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			return true;
		}
	}
	return false;
}

void UCInventoryComponent::ShowInventoryWidget()
{
	if (!InventoryWidget)
	{
		if (!CreateInventoryWidget())
		{
			UE_LOG(LogItem, Error, TEXT("Can't craete InventoryWidget."));
			return;
		}
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
		if (MyPC)
		{
			MyPC->ShowWidget(InventoryWidget);
		}
	}
}

void UCInventoryComponent::HideInventoryWidget()
{
	if (InventoryWidget)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			ACPlayerController* MyPC = Cast<ACPlayerController>(PC);
			if (MyPC)
			{
				MyPC->HideWidget(InventoryWidget);
			}
		}
	}

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
			Slot.ItemClass = InItemActor->GetClass();
			Slot.UseAmount = InItemData.UseAmount;

			if (ACItem_Consumable* Consumable = Cast<ACItem_Consumable>(InItemActor))
			{
				Slot.ConsumableType = Consumable->ConsumableType;
			}
			else
			{
				Slot.ConsumableType = EConsumableType::None;
			}
			InItemActor->ReduceAvailableCount(AmountToAdd);
			ChangedIndexes.AddUnique(i);

			if ((InItemAssetData.ItemIconTextureRef).IsEmpty())
			{
				UE_LOG(LogItem, Error, TEXT("ItemIconTexture Reference is missing. ItemID : %s"), *(UEnum::GetValueAsString(Slot.ItemID)));
			}
			else
			{
				CHelpers::GetAssetDynamic(&Slot.ItemIcon, InItemAssetData.ItemIconTextureRef);
			}
			Slot.Description = InItemAssetData.Description;
		}
		else
		{
			UE_LOG(LogItem, Error, TEXT("ItemID : %s"), *(UEnum::GetValueAsString(Slot.ItemID)));
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

void UCInventoryComponent::ToggleTick(bool IsTickEnable)
{
	SetComponentTickEnabled(IsTickEnable);
}

void UCInventoryComponent::ShowWarningWidget(FString Message)
{
	if (AController* Controller = OwnerCharacter->GetController())
	{
		if (ACPlayerController* MyPC = Cast<ACPlayerController>(Controller))
		{
			MyPC->ShowWarningWidget(Message);
		}
	}
}

void UCInventoryComponent::SwapSlot(const int32& SlotIndex1, const int32& SlotIndex2)
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
	OnInventorySlotSwap.Broadcast(SlotIndex1, SlotIndex2);
	OnInventorySlotDataUpdated.Broadcast(ChangedIndexes);
}

void UCInventoryComponent::UseItem(const int32& SlotIndex)
{

}

void UCInventoryComponent::DropItem(int32 InIndex)
{
	// 버리는 아이템 생성
	if (InventorySlots.IsValidIndex(InIndex))
	{
		TSubclassOf<ACItemBase> ItemClass = InventorySlots[InIndex].ItemClass;
		FTransform SpawnTransform = GetOwner()->GetTransform();
		if (UCameraComponent* CameraComp = OwnerCharacter->GetCameraComponent())
		{
			FVector ForwardVec = CameraComp->GetForwardVector();
			SpawnTransform.SetLocation(SpawnTransform.GetLocation() + ForwardVec * 50.0f);
		}
		ACItemBase* Item = GetWorld()->SpawnActorDeferred<ACItemBase>(ItemClass, SpawnTransform);
		Item->FinishSpawning(SpawnTransform);
		Item->SetMaxDurability(InventorySlots[InIndex].MaxDurability, false);
		Item->SetCurrentDurability(InventorySlots[InIndex].CurrentDurability);
		Item->SetAvailableCount(InventorySlots[InIndex].CurrentStack);

		if (ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter))
		{
			if (ACItemBase* EquippedItem=Player->GetCurretnEquippedItem())
			{
				if (EquippedItem->GetTargetSlotIndex() == InIndex)
				{
					Player->ResetEquippedItem();
				}
			}
		}
		ClearSlot(InIndex);
	}
}

void UCInventoryComponent::ClearSlot(int32 InIndex)
{
	if (InventorySlots.IsValidIndex(InIndex))
	{
		// Reduce Capacity
		if (UGameInstance* Instance = GetOwner()->GetGameInstance())
		{
			if (UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance))
			{
				TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(InventorySlots[InIndex].ItemID);
				if (ItemDataOpt.IsSet())
				{
					const FItemData& ItemData = ItemDataOpt.GetValue();
					CurrentCapacity -= InventorySlots[InIndex].CurrentStack * ItemData.ItemWeight;
				}
			}
		}
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

