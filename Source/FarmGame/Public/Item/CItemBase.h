#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CItemInterface.h"
#include "Interfaces/CInterface_Interactable.h"
#include "CItemBase.generated.h"

class USphereComponent;

UCLASS(Abstract) // 직접 인스턴스화 불가능하게 설정
class FARMGAME_API ACItemBase : public AActor, public ICItemInterface,public ICInterface_Interactable
{
	GENERATED_BODY()

public:
	ACItemBase();

public:
	// Inherited from ICItemInterface
	virtual bool IsUsable() const override { return bUsable; }
	virtual void SetUsable() override;
	virtual void SetUnusable() override;
	virtual bool UseItem() override PURE_VIRTUAL(ACItemBase::UseItem, return false;);

	virtual void AddAvailableCount(int32 InAmount = 1) override;
	virtual void ReduceAvailableCount(int32 InAmount = 1) override { AvailableCount = FMath::Max(0, AvailableCount - InAmount); }
	virtual int32 GetAvailableCount() const override { return AvailableCount; }

	// Inherited from ICInterface_Interactable
	virtual bool IsInteractable() override { return bInteractable; }
	virtual void SetInteractable() override;
	virtual void SetUnInteractable() override;
	virtual EInteractObjectType GetType() override { return InteractType; }
	virtual FName GetInteractName() override;
	virtual void SetType(EInteractObjectType InNewType) override;
	virtual void Interact(AActor* OtherActor) PURE_VIRTUAL(ACItemBase::Interact, return;);
	virtual bool OnHovered() PURE_VIRTUAL(ACItemBase::OnHovered, return false;);
	virtual bool OnUnhovered() PURE_VIRTUAL(ACItemBase::OnUnhovered, return false;);


	// ItemData
	void SetAvailableCnt(int32 cnt);
	int32 GetAvailableCount() { return AvailableCount; }
protected:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComp;

	// 아이템 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemID ItemID;

	// 사용 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	bool bUsable;

	// 현재 보유 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	int32 AvailableCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	EQualityType ItemQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;
};
