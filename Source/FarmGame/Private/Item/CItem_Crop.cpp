#include "Item/CItem_Crop.h"
#include "Components/CInventoryComponent.h"
#include "CGameInstance.h"

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
		UGameInstance* Instance = GetGameInstance();
		if (Instance)
		{
			UCGameInstance* MyInstance = Cast<UCGameInstance>(Instance);
			if (MyInstance)
			{
				TOptional<FItemData> ItemDataOpt = MyInstance->GetItemtData(ItemID);
				if (ItemDataOpt.IsSet())
				{
					if (InventoryComp->AddItem(ItemDataOpt.GetValue(), AvailableCount))
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
