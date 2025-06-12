#include "Notifies/CAnimNotify_Plant.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CInteractComponent.h"

FString UCAnimNotify_Plant::GetNotifyName_Implementation() const
{
    return "Plant_Notify";
}

void UCAnimNotify_Plant::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    CheckNull(MeshComp);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (Player != nullptr)
    {
        Player->OnPlantingAnimationFinished();
    }
}