#include "Notifies/CAnimNotifyState_Hoeing.h"
#include "Characters/CPlayer.h"
#include "NiagaraComponent.h"
#include "Item/CItem_Tool.h"

FString UCAnimNotifyState_Hoeing::GetNotifyName_Implementation() const
{
    return "NotifyState_Hoeing";
}

void UCAnimNotifyState_Hoeing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner()))
    {
        if (ACItem_Tool* Tool = Cast<ACItem_Tool>(Player->GetCurretnEquippedItem()))
        {
            Tool->UseItem();
        }
    }
}

void UCAnimNotifyState_Hoeing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner()))
    {
        if (ACItem_Tool* Tool = Cast<ACItem_Tool>(Player->GetCurretnEquippedItem()))
        {
            Tool->EndUse();
        }
        Player->OnHoeingAnimationFinished();
    }
}


