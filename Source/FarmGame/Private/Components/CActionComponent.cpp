#include "Components/CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Actions/CActionData.h"
//#include "Actions/CEquipment.h"
//#include "Actions/CDoAction.h"

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	/*ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if (OwnerCharacter)
	{
		for (int32 i = 0; i < (int32)EActionType::Max; i++)
		{
			if (!DataAssets[i])continue;
			DataAssets[i]->BeginPlay(OwnerCharacter);
		}
	}*/
}

void UCActionComponent::PrimaryAction()
{
	CheckTrue(IsUnarmedMode());

	/*if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetDoAction())
	{
		DataAssets[(int32)Type]->GetDoAction()->PrimaryAction();
	}*/
}

void UCActionComponent::SetUnarmedMode()
{
	/*if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetEquipment())
	{
		DataAssets[(int32)Type]->GetEquipment()->Unequip();
	}

	if (DataAssets[(int32)EActionType::Unarmed] && DataAssets[(int32)EActionType::Unarmed]->GetEquipment())
	{
		DataAssets[(int32)EActionType::Unarmed]->GetEquipment()->Equip();
	}*/
	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetRightHandMode()
{
	SetMode(EActionType::RightHand);
}

void UCActionComponent::SetLeftHandMode()
{
	SetMode(EActionType::LeftHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnarmedMode();
		return;
	}
	else if (!IsUnarmedMode())
	{
		/*if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetEquipment())
		{
			DataAssets[(int32)Type]->GetEquipment()->Unequip();
		}*/
	}

	/*if (DataAssets[(int32)InNewType] && DataAssets[(int32)InNewType]->GetEquipment())
	{
		DataAssets[(int32)InNewType]->GetEquipment()->Equip();
	}*/

	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType PrevType = Type;
	Type = InNewType;

	OnActionTypeChanged.Broadcast(PrevType, Type);
}
