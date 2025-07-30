#include "Item/CItem_Tool.h"	
#include "Characters/CPlayer.h"

ACItem_Tool::ACItem_Tool()
{
    UseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UseEffect"));
    UseEffect->SetupAttachment(MeshComp);
    UseEffect->SetAutoActivate(false);

    ConsumedDurability = 1.0f;
    SetType(EInteractObjectType::Tool);
    SetInteractable();
    SetUsable();
}

bool ACItem_Tool::UseItem()
{
    if (!IsUsable()) return false;
    if (!IsValid(GetOwnerCharacter())) return false;
    if (CurrentDurability <= 0.0f)return false;
    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
    {
        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
        {
            InventoryComp->ReduceItemDurability(GetTargetSlotIndex(), ConsumedDurability);
            CurrentDurability -= ConsumedDurability;
            
            return true;
        }
    }
    return false;
}

void ACItem_Tool::EndUse()
{
    if (CurrentDurability <= 0.0f)
    {
        if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
        {
            Player->ResetEquippedItem();
            this->Destroy();
        }
    }
}

void ACItem_Tool::PlayAnimation()
{
    return;
}
