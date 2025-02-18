#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CItemInterface.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,   // �Ҹ�ǰ (��: ����, ���)
	Crop,         // ��Ȯ�� (��: �丶��, ����)
	Tool,         // ���� (��: ���, ����)
	Miscellaneous // ��Ÿ (��: ����Ʈ ������)
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
	EQualityType ItemQuality; // ǰ�� �߰�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price; // ǰ���� ����
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
