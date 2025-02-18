#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CItemInterface.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,   // 소모품 (예: 씨앗, 비료)
	Crop,         // 수확물 (예: 토마토, 감자)
	Tool,         // 도구 (예: 곡괭이, 도끼)
	Miscellaneous // 기타 (예: 퀘스트 아이템)
};

UENUM(BlueprintType)
enum class EQualityType : uint8
{
	Low,
	Normal,
	High,
	MAX
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQualityType ItemQuality; // 품질 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price; // 품질별 가격
};

UINTERFACE(MinimalAPI)
class UCItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMGAME_API ICItemInterface
{
	GENERATED_BODY()
public:
	virtual bool IsUsable() const = 0;
	virtual void SetUsable() = 0;
	virtual void SetUnusable() = 0;
	virtual bool UseItem() = 0;

	virtual void AddAvailableCount(int32 InAmount = 1) = 0;
	virtual void ReduceAvailableCount(int32 InAmount = 1) = 0;
	virtual int32 GetAvailableCount() const = 0;
};
