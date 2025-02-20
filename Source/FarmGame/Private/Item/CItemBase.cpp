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
	UGameInstance* Instance = GetGameInstance();
	if (Instance)
	{
		UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
		if (MyInstance)
		{

			TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(ItemID);
			if (ItemDataOpt.IsSet())
			{
				FItemData& ItemData = ItemDataOpt.GetValue();
				return ItemData.ItemName;
			}
		}
	}
	UE_LOG(LogItem, Error, TEXT("Error! Can't find ItemName! ItemID : %s"), *(UEnum::GetValueAsString(ItemID)));
	return TEXT("Error! Can't find ItemName!");
}

void ACItemBase::SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}

void ACItemBase::SetAvailableCnt(int32 InCnt)
{
	AvailableCount = InCnt;
}
