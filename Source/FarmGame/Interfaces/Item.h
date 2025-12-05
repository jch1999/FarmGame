#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);

UENUM(BlueprintType)
enum class EConsumableType : uint8
{
	None,
	Seed,        // 씨앗
	Fertilizer,  // 비료
	Potion       // 포션, 기타 소모품
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,   // 소모품 (예: 씨앗, 비료)
	Crop,         // 수확물 (예: 토마토, 감자)
	Tool,         // 도구 (예: 곡괭이, 도끼)
	Miscellaneous // 기타 (예: 퀘스트 아이템)
};

UENUM(BlueprintType)
enum class EItemID :uint8
{
	None,
	// Crop
	Tomato_Low,
	Tomato_Normal,
	Tomato_High,
	Berry_Low,
	Berry_Normal,
	Berry_High,
	Carrot_Low,
	Carrot_Normal,
	Carrot_High,
	// Consumable
	// Consumable - Seed
	Seed_Tomato,
	Seed_Berry,
	Seed_Carrot,
	// Consumable - Fertilizer
	Fertilizer,
	// Tool
	WaterCan,
	Hoe,
	// End
	MAX
};

UENUM(BlueprintType)
enum class EQualityType : uint8
{
	None,
	Low,
	Normal,
	High,
	MAX
};

USTRUCT(BlueprintType)
struct FItemAssetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemID ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemClassRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemIconTextureRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ParticleEffectRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SoundEffectRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemID ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PurchasePrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseAmount;
};

USTRUCT(BlueprintType)
struct FItemAttachData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attach")
	EItemID ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attach")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attach")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attach")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attach")
	FVector Scale;
};

UINTERFACE(MinimalAPI)
class UItem : public UInterface
{
	GENERATED_BODY()
};

class FARMGAME_API IItem
{
	GENERATED_BODY()
public:

	virtual void AddAvailableCount(int32 InAmount = 1) = 0;
	virtual void SetAvailableCount(int32 InAmount) = 0;
	virtual void ReduceAvailableCount(int32 InAmount = 1) = 0;
	virtual const int32& GetAvailableCount() const = 0;
	virtual	void SetTargetSlotIndex(int Index) = 0;
	virtual	const int32& GetTargetSlotIndex() = 0;
	virtual void SetMaxDurability(float InDurability, bool bReset = false) = 0;
	virtual const float& GetMaxDurability() = 0;
	virtual const float& GetCurrentDurability() = 0;
	virtual void SetCurrentDurability(float InDurability) = 0;
};
