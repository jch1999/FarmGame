#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;
class UCInventoryWidget;
class UCQuickSlotBarWidget;

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
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	FInventorySlot()
		: ItemID(EItemID::None), CurrentStack(0), MaxStackSize(1), ItemIcon(nullptr),Description() {
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotDataUpdated, const TArray<int32>&, ChangedIndexs);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySlotCountUpdated, int32, IncreasedCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMGAME_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool AddItem(FItemData& InItemData, int32& InCount);
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
	bool AddToExistingSlot(FItemData& InItemData, uint8& InCount);
	bool AddToNewSlot(FItemData& InItemData, uint8& InCount);
	UFUNCTION()
	bool IncreaseSlotCount(int32 InSlotCount);

public:
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
	
	UPROPERTY(BlueprintAssignable)
	FInventorySlotDataUpdated OnInventorySlotDataUpdated;

	UPROPERTY(BlueprintAssignable)
	FInventorySlotCountUpdated OnInventorySlotCountUpdated;
};
