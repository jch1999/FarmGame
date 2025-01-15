#include "Actions/CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CEquipment.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	if (AttachmentClass)
	{
		FTransform TM;
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, TM, InOwnerCharacter, nullptr);

#if WITH_EDITOR
		Attachment->SetActorLabel(MakeLabel(InOwnerCharacter, "Attachment"));
#endif
		Attachment->FinishSpawning(TM);
	}

	if (EquipmentClass)
	{
		FTransform TM;
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, TM, InOwnerCharacter, nullptr);

#if WITH_EDITOR
		Equipment->SetActorLabel(MakeLabel(InOwnerCharacter, "Equipment"));
#endif
		Equipment->SetData(EquipmentData);
		Equipment->FinishSpawning(TM);

		if (Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	if (DoActionClass)
	{
		FTransform TM;
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, TM, InOwnerCharacter, nullptr);

#if WITH_EDITOR
		DoAction->SetActorLabel(MakeLabel(InOwnerCharacter, "DoAction"));
#endif
		DoAction->SetDatas(DoActionDatas);
		DoAction->FinishSpawning(TM);

		if (Attachment)
		{
			
		}
	}
}

FString UCActionData::MakeLabel(ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString Label;

#if WITH_EDITOR
	Label.Append(InOwnerCharacter->GetActorLabel());
#endif
	Label.Append("_");
	Label.Append(InMiddleName);
	Label.Append(GetName().Replace(TEXT("DA"), TEXT("")));

	return Label;
}
