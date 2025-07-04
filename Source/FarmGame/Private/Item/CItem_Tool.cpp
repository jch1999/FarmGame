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
    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
    {
        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
        {
            InventoryComp->ReduceItemDurability(GetTargetSlotIndex(), ConsumedDurability);
            ConsumedDurability -= ConsumedDurability;
        }
    }
    return false;
}

void ACItem_Tool::EndUse()
{

}

void ACItem_Tool::PlayAnimation()
{
    return;
}
