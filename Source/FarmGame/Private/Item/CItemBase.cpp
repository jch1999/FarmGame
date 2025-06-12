#include "Item/CItemBase.h"
#include "Components/SphereComponent.h"
#include "Global.h"
#include "CGameInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/CInventoryComponent.h"

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

	SetType(EInteractObjectType::Item);
	SetInteractable();
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();

	// 질량 설정
	if (MeshComp)
	{
		// Reduce Restitution
		UPhysicalMaterial* PhysMaterial = NewObject<UPhysicalMaterial>();
		PhysMaterial->Restitution = 0.0f; // 반발력 최소화
		PhysMaterial->Friction = 1.2f;    // 적절한 마찰력 부여
		MeshComp->SetPhysMaterialOverride(PhysMaterial); 
		MeshComp->SetLinearDamping(1.5f);   // 공기 저항 효과 적용 (기본값 0 → 1.5)
		MeshComp->SetAngularDamping(2.0f);  // 회전 감속 효과 적용 (기본값 0 → 2.0)
	}
}

void ACItemBase::SetUsable()
{
	bUsable = true;
}

void ACItemBase::SetUnusable()
{
	bUsable = false;
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
