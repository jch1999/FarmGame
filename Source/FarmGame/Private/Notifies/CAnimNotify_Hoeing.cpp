#include "Notifies/CAnimNotify_Hoeing.h"
#include "Characters/CPlayer.h"
#include "Item/CItem_Tool.h"

FString UCAnimNotify_Hoeing::GetNotifyName_Implementation() const
{
	return "Hoeing_Notify";
}

void UCAnimNotify_Hoeing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    CheckNull(MeshComp);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (Player != nullptr)
    {
        Player->OnHoeingAnimationFinished();
        if (ACItem_Tool* Tool = Cast<ACItem_Tool>(Player->GetCurretnEquippedItem()))
        {
            Tool->EndUse();
        }
    }
}
