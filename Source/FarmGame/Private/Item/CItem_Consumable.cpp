#include "Item/CItem_Consumable.h"
#include "Controller/CPlayerController.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"

bool ACItem_Consumable::UseItem()
{
    if (!IsUsable()) return false;
    if (!IsValid(GetOwnerCharacter())) return false;
    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
    {
        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
        {
            InventoryComp->ReduceItemStack(GetTargetSlotIndex());
        }
    }
    return false;
}