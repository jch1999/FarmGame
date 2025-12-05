#include "Item/CItem_Tool.h"	
#include "Characters/CPlayer.h"
#include "CGameInstance.h"

ACItem_Tool::ACItem_Tool()
{
    UseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UseEffect"));
    UseEffect->SetupAttachment(MeshComp);
    UseEffect->SetAutoActivate(false);

    ConsumedDurability = 1.0f;
    //SetMaxDurability(50.0f, true);
    SetType(EInteractObjectType::Tool);
    SetInteractable();
    IUsableItem::Execute_SetUsable(this);
}

bool ACItem_Tool::UseItem_Implementation()
{
    if (!IsUsable()) return false;
    if (!IsValid(GetOwnerCharacter())) return false;
    if (CurrentDurability <= 0.0f)return false;
    if (ACPlayer* Player = Cast<ACPlayer>(GetOwnerCharacter()))
    {
        if (UCInventoryComponent* InventoryComp = Player->GetInventoryComponent())
        {
            InventoryComp->ReduceItemDurability(GetTargetSlotIndex(), ConsumedDurability);
            //CurrentDurability -= ConsumedDurability;
            
            return true;
        }
    }
    return false;
}

void ACItem_Tool::EndUse_Implementation()
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

bool ACItem_Tool::PlayAnimation_Implementation()
{
    return true;
}

bool ACItem_Tool::IsUsable_Implementation() const
{
    return bUsable;
}

void ACItem_Tool::SetUsable_Implementation()
{
    bUsable = true;
}

void ACItem_Tool::SetUnusable_Implementation()
{
    bUsable = false;
}
