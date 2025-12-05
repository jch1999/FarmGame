#include "Item/CItem_Consumable.h"
#include "Controller/CPlayerController.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"

//bool ACItem_Consumable::UseItem_Implementation()
//{
//    if (!IsUsable()) return false;
//    if (!IsValid(GetOwnerCharacter())) return false;
//    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
//    {
//        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
//        {
//            InventoryComp->ReduceItemStack(GetTargetSlotIndex());
//            // ReduceAvailableCnt(1);
//            if (GetAvailableCount() == 0)
//            {
//                Player->ResetEquippedItem();
//                this->Destroy();
//            }
//        }
//    }
//    return false;
//}

bool ACItem_Consumable::UseItem_Common()
{
    if (!IsValid(GetOwnerCharacter())) return false;
    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
    {
        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
        {
            InventoryComp->ReduceItemStack(GetTargetSlotIndex());
            // ReduceAvailableCnt(1);
            if (GetAvailableCount() == 0)
            {
                Player->ResetEquippedItem();
                this->Destroy();
            }
        }
    }
    return false;
}
