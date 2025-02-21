#include "Item/CItemBase.h"
#include "Components/SphereComponent.h"
#include "Global.h"
#include "CGameInstance.h"

ACItemBase::ACItemBase()
{
	// Mesh Component
	CHelpers::CreateSceneComponent(this, &MeshComp, "MeshComp", RootComponent);
	
	// Box Component
	CHelpers::CreateSceneComponent(this, &SphereComp, "SphereComp", RootComponent);
	SphereComp->SetSphereRadius(64.0f);
	SphereComp->SetCollisionProfileName(TEXT("InteractObject"));

	SetType(EInteractObjectType::Item);
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
