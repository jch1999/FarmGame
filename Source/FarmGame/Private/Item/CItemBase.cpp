#include "Item/CItemBase.h"
#include "Components/SphereComponent.h"
#include "Global.h"

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
	TOptional<FItemData> ItemDataOpt = GetItemtData(ItemName);
	if (ItemDataOpt.IsSet())
	{
		FItemData& ItemData = ItemDataOpt.GetValue();
		return ItemData.ItemName;
	}

	return TEXT("Error! Can't find ItemName!");
}

void ACItemBase::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}
