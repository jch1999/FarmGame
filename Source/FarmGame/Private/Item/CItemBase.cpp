#include "Item/CItemBase.h"
#include "Components/SphereComponent.h"
#include "Global.h"
#include "CGameInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/CInventoryComponent.h"
#include "Characters/CPlayer.h"

ACItemBase::ACItemBase()
{
	// Mesh Component
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	//MeshComp->SetSimulatePhysics(true);
	MeshComp->SetMassOverrideInKg(NAME_None, 100.0f);
	//MeshComp->SetCollisionProfileName(TEXT("InteractObject"));
	MeshComp->BodyInstance.bUseCCD = true; 
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetCollision(true);
	// Sphere Component
	/*CHelpers::CreateSceneComponent(this, &SphereComp, "SphereComp", RootComponent);
	SphereComp->SetSphereRadius(30.0f);
	SphereComp->SetCollisionProfileName(TEXT("InteractObject"));*/

	SetAvailableCnt(1);
	SetMaxDurability(0.0f, false);
	SetType(EInteractObjectType::Item);
	SetInteractable();

	SlotIndex = -1;
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();

	// 질량 설정
	if (MeshComp)
	{
		// Reduce Restitution
		UPhysicalMaterial* PhysMaterial = nullptr;// = NewObject<UPhysicalMaterial>();
		//PhysMaterial->Restitution = 0.0f; // 반발력 최소화
		//PhysMaterial->Friction = 1.2f;    // 적절한 마찰력 부여
		CHelpers::GetAssetDynamic(&PhysMaterial, "/Game/Materials/PM_Item");
		MeshComp->SetPhysMaterialOverride(PhysMaterial); 
		MeshComp->SetLinearDamping(1.5f);   // 공기 저항 효과 적용 (기본값 0 → 1.5)
		MeshComp->SetAngularDamping(2.0f);  // 회전 감속 효과 적용 (기본값 0 → 2.0)
	}

	// Tool 타입일 때만 Durability 초기화
	if (InteractType == EInteractObjectType::Tool)
	{
		if (!HasAnyFlags(RF_Transient))
		{
			if (UCGameInstance* MyGI = Cast<UCGameInstance>(GetWorld()->GetGameInstance()))
			{
				TOptional<FItemData> ItemDataOpt = MyGI->GetItemtData(ItemID);
				if (ItemDataOpt.IsSet())
				{
					SetMaxDurability(ItemDataOpt.GetValue().MaxDurability, true);
					SetUseAmount(ItemDataOpt.GetValue().UseAmount);
				}
			}
		}
	}
}


void ACItemBase::AddAvailableCount(int32 InAmount)
{
	AvailableCount += InAmount;
}

void ACItemBase::ReduceAvailableCount(int32 InAmount)
{
	AvailableCount = FMath::Max(0, (AvailableCount - InAmount));
}

void ACItemBase::SetInteractable()
{
	bInteractable = true;
}

void ACItemBase::SetUnInteractable()
{
	bInteractable = false;
}

void ACItemBase::SetDelayedInteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACItemBase::SetInteractable, DelayTime, false);
}

void ACItemBase::SetDelayedUninteractable(float DelayTime)
{
	if (GetWorld()->GetTimerManager().TimerExists(InteractTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(InteractTimer, this, &ACItemBase::SetUnInteractable, DelayTime, false);
}

FName ACItemBase::GetInteractName()
{
	FString ItemFullName = UEnum::GetValueAsString(ItemID);
	ItemFullName.RemoveFromStart("EItemID::");
	FString ItemName, ItemExtra;
	if (ItemFullName.Split("_", &ItemName, &ItemExtra))
	{
		FString NameStr = ItemName + " (" + ItemExtra + ")";
		return FName(*NameStr);
	}
	else
	{
		return FName(*ItemFullName);
	}
	
	/*UE_LOG(LogItem, Error, TEXT("Error! Can't find ItemName! ItemID : %s"), *(UEnum::GetValueAsString(ItemID)));
	return TEXT("Error! Can't find ItemName!");*/
}

void ACItemBase::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACItemBase::SetAvailableCount(int32 InAmount)
{
	AvailableCount = InAmount;
}

void ACItemBase::SetAvailableCnt(int32 InCnt)
{
	AvailableCount = InCnt;
}

void ACItemBase::SetCollision(bool bEnabled)
{
	MeshComp->SetSimulatePhysics(bEnabled);
	if (bEnabled)
	{
		MeshComp->SetCollisionProfileName(TEXT("InteractObject"));
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACItemBase::Interact(AActor* OtherActor)
{
	if (UCInventoryComponent* InventoryComp = OtherActor->GetComponentByClass<UCInventoryComponent>())
	{
		UGameInstance* Instance = GetGameInstance();
		if (Instance)
		{
			UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
			if (MyInstance)
			{
				TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(ItemID);
				if (ItemDataOpt.IsSet())
				{
					if (InventoryComp->AddItem(this))
					{
						Destroy();
					}
				}
				else
				{
					UE_LOG(LogItem, Error, TEXT("Add Item failed on %s"), *(UEnum::GetValueAsString(ItemID)));
				}
			}
		}
	}
}

void ACItemBase::SetTargetSlotIndex(int Index)
{
	SlotIndex = Index;
	if (IsValid(OwnerCharacter))
	{
		TArray<int32> Indexes;
		Indexes.Add(SlotIndex);
		UpdateByInventory_DataUpdated(Indexes);
	}
}

void ACItemBase::SetOwnerCharacter(ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
}

void ACItemBase::ReduceAvailableCnt(int32 InCnt)
{
	AvailableCount -= InCnt;
	if (AvailableCount < 0)
	{
		AvailableCount = 0;
	}
}

void ACItemBase::UpdateByInventory_DataUpdated(const TArray<int32>& ChangedIndexs)
{
	if (IsValid(OwnerCharacter))
	{
		if (ChangedIndexs.Contains(SlotIndex))
		{
			if (ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter))
			{
				if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
				{
					const FInventorySlot& SlotData = InventoryComp->GetSlotDatas()[SlotIndex];
					SetAvailableCnt(SlotData.CurrentStack);
					SetMaxDurability(SlotData.MaxDurability, false);
					CurrentDurability = SlotData.CurrentDurability;
				}
			}
		}
	}
}

void ACItemBase::UpdateByInventory_SlotSwap(int32 Index1, int32 Index2)
{
	if (IsValid(OwnerCharacter))
	{
		if (Index1 == SlotIndex)
		{
			SlotIndex = Index2;
		}
		else if (Index2 == SlotIndex)
		{
			SlotIndex = Index1;
		}
	}
}

void ACItemBase::SetMaxDurability(float InDurability, bool bReset)
{
	MaxDurability = InDurability;
	if (bReset)
	{
		CurrentDurability = MaxDurability;
	}
}

void ACItemBase::SetCurrentDurability(float InDurability)
{
	CurrentDurability = InDurability;
}

void ACItemBase::SetUseAmount(float InAmount)
{
	UseAmount = InAmount;
}
