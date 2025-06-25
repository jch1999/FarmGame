#include "Notifies/CAnimNotifyState_Watering.h"
#include "Characters/CPlayer.h"
#include "NiagaraComponent.h"
#include "Item/CItem_Tool.h"

FString UCAnimNotifyState_Watering::GetNotifyName_Implementation() const
{
	return "NotifyState_Watering";
}

void UCAnimNotifyState_Watering::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner()))
    {
        if (ACItem_Tool* Tool = Cast<ACItem_Tool>(Player->GetCurretnEquippedItem()))
        {
            Tool->UseItem();
        }
    }
}

void UCAnimNotifyState_Watering::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner()))
    {
        if (ACItem_Tool* Tool = Cast<ACItem_Tool>(Player->GetCurretnEquippedItem()))
        {
            Tool->EndUse();
        }
        Player->OnWateringAnimationFinished();
    }
}
