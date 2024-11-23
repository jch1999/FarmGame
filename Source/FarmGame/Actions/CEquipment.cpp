#include "Actions/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"

ACEquipment::ACEquipment()
{
}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	AttributeComp = CHelpers::GetComponent<UCAttributeComponent>(OwnerCharacter);
}

void ACEquipment::SetData(FEquipmentData& InData)
{
	Data = InData;
}

void ACEquipment::Equip_Implementation()
{
	StateComp->SetEquipMode();
	Data.bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();

	if (Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate, Data.StartSection);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bUseControlRotation)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void ACEquipment::Begin_Equip_Implementation()
{
	OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	StateComp->SetIdleMode();
	AttributeComp->SetMove();
}

void ACEquipment::Unequip_Implementation()
{
	if (Data.bUseControlRotation)
	{
		OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	OnUnequipmentDelegate.Broadcast();
}
