#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CItemInterface.h"
#include "CInventoryComponent.generated.h"

class UUserWidget;

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

	FInventorySlot()
		: ItemID(EItemID::MAX), CurrentStack(0), MaxStackSize(1) {
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
	bool AddItem(FItemData& InItemData, uint8& InCount);
	void ShowWarningWidget(FString Message);
private:
	bool AddToExistingSlot(FItemData& InItemData, uint8& InCount);
	bool AddToNewSlot(FItemData& InItemData, uint8& InCount);

public:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float NowCapacity;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MaxCapacity;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxSlotCnt;

	// Widget
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> QuickSlotWidgetClass;
		
};
