#include "Item/CItemBase.h"
#include "Components/BoxComponent.h"

ACItemBase::ACItemBase()
{
	// Box Component
	BoxComp->SetBoxExtent(FVector(64.0f, 64.0f, 64.0f));
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	BoxComp->SetCollisionProfileName(TEXT("InteractObject"));
}

void ACItemBase::SetUsable()
{
	bUsable = true;
}

void ACItemBase::SetUnusable()
{
	bUsable = false;
}


void AddAvailableCount(int32 InAmount)
{
	AvailableCount += InAmount;
}

void SetInteractable()
{
	bInteractable = true;
}

void SetUnInteractable()
{
	bInteractable = false;
}

void SetType(EInteractObjectType InNewType)
{
	InteractType = InNewType;
}
