#include "Item/CItem_Tool.h"	
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Characters/CPlayer.h"

ACItem_Tool::ACItem_Tool()
{
    UseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("UseEffect"));
    UseEffect->SetupAttachment(MeshComp);
    UseEffect->SetAutoActivate(false);

    SetType(EInteractObjectType::Tool);
    SetInteractable();
    SetUsable();
}

bool ACItem_Tool::UseItem()
{
    return false;
}

void ACItem_Tool::SetOwnerPlayer(ACPlayer* InPlayer)
{
    if (InPlayer == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Set OwnerPlayer Failed. Unavailable Player!"));
    }
    OwnerPlayer = InPlayer;
}

const ACPlayer* ACItem_Tool::GetOwnerPlayer()
{
    return OwnerPlayer;
}
