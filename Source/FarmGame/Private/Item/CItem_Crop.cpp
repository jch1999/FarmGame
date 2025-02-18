#include "Item/CItem_Crop.h"
#include "Components/CInventoryComponent.h"
ACItem_Crop::ACItem_Crop()
{
	SetUnusable();
}

bool ACItem_Crop::SetCropData(FCropData& InData)
{
	CropData = InData;

	return true;
}

bool ACItem_Crop::UseItem()
{
	return false;
}

void ACItem_Crop::Interact(AActor* OtherActor)
{
	UCInventoryComponent* InventoryComp = OtherActor->GetComponentByClass<UCInventoryComponent>();
	if (InventoryComp)
	{
		TOptional<FItemData> ItemDataOpt = GetItemtData(ItemName);
		if (ItemDataOpt.IsSet())
		{
			InventoryComp->AddItem(ItemDataOpt.GetValue(), AvailableCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Add Item failed on %s"), *ItemName.ToString());
		}
	}
	Destroy();
}
