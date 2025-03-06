#include "Item/CItemBase.h"
#include "Components/SphereComponent.h"
#include "Global.h"
#include "CGameInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ACItemBase::ACItemBase()
{
	// Mesh Component
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetMassOverrideInKg(NAME_None, 100.0f);
	MeshComp->SetCollisionProfileName(TEXT("InteractObject"));
	MeshComp->BodyInstance.bUseCCD = true; 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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
		PhysMaterial->Friction = 0.8f;    // 적절한 마찰력 부여
		MeshComp->SetPhysMaterialOverride(PhysMaterial);
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
