#include "Components/CInventoryComponent.h"
#include "Blueprint/UserWidget.h"
UCInventoryComponent::UCInventoryComponent()
{
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCInventoryComponent::AddItem(FItemData& InItemData, int32& InCount)
{
	return false;
}
