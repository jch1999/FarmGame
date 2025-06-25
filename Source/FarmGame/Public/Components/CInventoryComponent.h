#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;
class UCInventoryWidget;
class UCQuickSlotBarWidget;
class ACItemBase;
class ACPlayer;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemID ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 CurrentStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxStackSize;  // FItemData에서 가져올 예정

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConsumableType ConsumableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACItemBase> ItemClass;

	FInventorySlot()
		: ItemID(EItemID::None), CurrentStack(0), MaxStackSize(1), MaxDurability(0.0f), CurrentDurability(0.0f), ItemIcon(nullptr),Description(),
		ConsumableType(EConsumableType::None), ItemClass(nullptr){
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotDataUpdated, const TArray<int32>&, ChangedIndexs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventorySlotSwap, int32, Index1, int32, Index2);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotCountUpdated, int32, IncreasedCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FITemAdded, FName, InItemName, int32, InItemAmount, UTexture2D*, InItemIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoneyChanged, int32, NewValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DelatTime, enum ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool AddItem(ACItemBase* InItemActor);
	bool ReduceItemStack(int InIndex);
	bool ReduceItemDurability(int InIndex, float ConsumedDurability);

	UFUNCTION()
	void ShowInventoryWidget();
	UFUNCTION()
	void HideInventoryWidget();
	void ShowWarningWidget(FString Message);

	void SwapSlot(int32& SlotIndex1, int32& SlotIndex2);
	void UseItem(int32& SlotIndex);
	void DropItem(int32 InIndex);;
	void ClearSlot(int32 InIndex);
	FORCEINLINE const TArray<FInventorySlot>& GetSlotDatas() { return InventorySlots; }
	TWeakPtr<FInventorySlot> GetSlotWeak(int32 Index);

	// void SetQuickSlotIndex(int32 InIndex);
	UFUNCTION()
	FORCEINLINE int32 GetMoney() { return OwnMoney; }

	UFUNCTION()
	bool AddMoney(int32 InGetAmount);
	
	UFUNCTION()
	bool UseMoney(int32 InUsedAmount);

private:
	bool CreateInventoryWidget();

	bool AddToExistingSlot(ACItemBase* InItemActor, TArray<int32>& ChangedIndexes);
	bool AddToNewSlot(ACItemBase* InItemActor, const FItemData& InItemData, const FItemAssetData& InItemAssetData, TArray<int32>& ChangedIndexes);
	UFUNCTION()
	bool IncreaseSlotCount(int32 InSlotCount);
	UFUNCTION()
	void ToggleTick(bool IsTickEnable);

public:
	UPROPERTY(BlueprintAssignable)
	FInventorySlotDataUpdated OnInventorySlotDataUpdated;

	UPROPERTY(BlueprintAssignable)
	FInventorySlotSwap OnInventorySlotSwap;

	UPROPERTY(BlueprintAssignable)
	FInventorySlotCountUpdated OnInventorySlotCountUpdated;

	UPROPERTY(BlueprintAssignable)
	FITemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FMoneyChanged OnMoneyUpdated;

protected:
	UPROPERTY()
	ACPlayer* OwnerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Inventory|Limits")
	float CurrentCapacity;
	
	UPROPERTY(EditAnywhere, Category = "Inventory|Limits")
	float MaxCapacity;
	
	UPROPERTY(EditAnywhere, Category = "Inventory|Limits")
	int32 CurrentSlotCnt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Limits")
	int32 MaxSlotCnt;

	UPROPERTY(EditAnywhere, Category = "Inventory|QuickSlot")
	int32 CurrentQuickSlotIndex;

	// Widget
	UPROPERTY(EditAnywhere, Category = "Widget|Inventory")
	TSubclassOf<UCInventoryWidget> InventoryWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "Widget|Inventory")
	UCInventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget|QuickSlot")
	UCQuickSlotBarWidget* QuickSlotBar;
	
	UPROPERTY(VisibleAnywhere, Category="Money")
	int32 OwnMoney;

private:
	int32 DefaultSlotCnt;
};
