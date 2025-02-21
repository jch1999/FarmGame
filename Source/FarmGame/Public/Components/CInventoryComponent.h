#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdated);

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
	void ShowWarningWidget(FString Message);
	void SwapSlot(int32& SlotIndex1, int32& SlotIndex2);
	void UseItem(int32& SlotIndex);

private:
	bool AddToExistingSlot(FItemData& InItemData, uint8& InCount);
	bool AddToNewSlot(FItemData& InItemData, uint8& InCount);

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

	// Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> QuickSlotWidgetClass;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryUpdated OnInventoryUpdated;
};
