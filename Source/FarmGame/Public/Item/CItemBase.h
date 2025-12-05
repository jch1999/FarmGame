#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Item.h"
#include "Interfaces/Interactable.h"
#include "CItemBase.generated.h"

class USphereComponent;

UCLASS(Abstract) // 직접 인스턴스화 불가능하게 설정
class FARMGAME_API ACItemBase : public AActor, public IItem,public IInteractable
{
	GENERATED_BODY()

public:
	ACItemBase();

protected:
	virtual void BeginPlay() override;

public:
	// About Item Data Inherited from ICItemInterface
	virtual void AddAvailableCount(int32 InAmount = 1) override;
	virtual void ReduceAvailableCount(int32 InAmount = 1) override;
	virtual const int32& GetAvailableCount() const override { return AvailableCount; }
	virtual void SetAvailableCount(int32 InAmount);
	virtual void SetTargetSlotIndex(int Index) override;
	virtual	const int32& GetTargetSlotIndex() override { return SlotIndex; }
	virtual void SetMaxDurability(float InDurability, bool bReset);
	virtual const float& GetMaxDurability() { return MaxDurability; }
	virtual const float& GetCurrentDurability() { return CurrentDurability; }
	virtual void SetCurrentDurability(float InDurability);

	// Inherited from ICInterface_Interactable
	virtual bool IsInteractable() override { return bInteractable; }
	virtual void SetInteractable() override;
	virtual void SetUnInteractable() override;

	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetDelayedInteractable(float DelayTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "InteracteInterface")
	virtual void SetDelayedUninteractable(float DelayTime) override;

	virtual EInteractObjectType GetType() override { return InteractType; }
	virtual FName GetInteractName() override;
	virtual void SetType(EInteractObjectType InNewType) override;
	// virtual void Interact(AActor* OtherActor) PURE_VIRTUAL(ACItemBase::Interact, return;);
	virtual void Interact(AActor* OtherActor);
	virtual bool OnHovered() PURE_VIRTUAL(ACItemBase::OnHovered, return false;);
	virtual bool OnUnhovered() PURE_VIRTUAL(ACItemBase::OnUnhovered, return false;);


	// ItemData
	EItemID GetItemID() { return ItemID; }

	void SetAvailableCnt(int32 InCnt);
	void ReduceAvailableCnt(int32 InCnt);

	float GetCurrentDruability() { return CurrentDurability; }

	void SetCollision(bool bEnabled);
	
	void SetOwnerCharacter(ACharacter* InOwnerCharacter);
	ACharacter* GetOwnerCharacter() { return OwnerCharacter; }

	UStaticMeshComponent* GetItemMesh() { return MeshComp; }

	UFUNCTION()
	void UpdateByInventory_DataUpdated(const TArray<int32>& ChangedIndexs);
	UFUNCTION()
	void UpdateByInventory_SlotSwap(int32 Index1, int32 Index2);

	void SetUseAmount(float InAmount);
	float GetUseAmount() { return UseAmount; }

protected:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//USphereComponent* SphereComp;

	// 아이템 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemID ItemID;

	// 현재 보유 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	int32 AvailableCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	EQualityType ItemQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	float MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	float CurrentDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	bool bInteractable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractInterface")
	EInteractObjectType InteractType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInterface")
	float UseAmount;

private:
	FTimerHandle InteractTimer;

	UPROPERTY(EditInstanceOnly,Category="Item|InventorySlotIndex")
	int SlotIndex;
	UPROPERTY(VisibleAnywhere,Category="Item|Owner")
	ACharacter* OwnerCharacter;
};
