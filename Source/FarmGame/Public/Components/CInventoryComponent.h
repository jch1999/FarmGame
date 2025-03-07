#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;
class UCInventoryWidget;
class UCQuickSlotBarWidget;
class ACItemBase;

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

	FInventorySlot()
		: ItemID(EItemID::None), CurrentStack(0), MaxStackSize(1), MaxDurability(0.0f), CurrentDurability(0.0f), ItemIcon(nullptr),Description() {
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotDataUpdated, const TArray<int32>&, ChangedIndexs);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotCountUpdated, int32, IncreasedCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FITemAdded, FName, InItemName, int32, InItemAmount, UTexture2D*, InItemIcon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool AddItem(ACItemBase* InItemActor);
	void ShowInventory();
	void HideInventory();
	void ShowWarningWidget(FString Message);
	void SwapSlot(int32& SlotIndex1, int32& SlotIndex2);
	void UseItem(int32& SlotIndex);
	void ClearSlot(int32 InIndex);
	FORCEINLINE const TArray<FInventorySlot>& GetSlotDatas() { return InventorySlots; }
	TWeakPtr<FInventorySlot> GetSlotWeak(int32 Index);

	// void SetQuickSlotIndex(int32 InIndex);
private:
	bool CreateInventoryWidget();
	bool AddToExistingSlot(ACItemBase* InItemActor, TArray<int32>& ChangedIndexes);
	bool AddToNewSlot(ACItemBase* InItemActor, const FItemData& InItemData, const FItemAssetData& InItemAssetData, TArray<int32>& ChangedIndexes);
	UFUNCTION()
	bool IncreaseSlotCount(int32 InSlotCount);

public:
	UPROPERTY(BlueprintAssignable)
	FInventorySlotDataUpdated OnInventorySlotDataUpdated;

	UPROPERTY(BlueprintAssignable)
	FInventorySlotCountUpdated OnInventorySlotCountUpdated;

	UPROPERTY(BlueprintAssignable)
	FITemAdded OnItemAdded;

protected:
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

	UPROPERTY(EditAnywhere, Category = "Widget|QuickSlot")
	TSubclassOf<UCQuickSlotBarWidget> QuickSlotBarWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "Widget|QuickSlot")
	UCQuickSlotBarWidget* QuickSlots;
	
	
private:
	int32 DefaultSlotCnt;
};
