#include "Components/CAttributeComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	WalkSpeeds[(int32)ESpeedType::Sneak] = 200.0f;
	WalkSpeeds[(int32)ESpeedType::Walk] = 400.0f;
	WalkSpeeds[(int32)ESpeedType::Sprint] = 600.0f;

	MaxHealth = 30.0f;
	CurrentHealth = MaxHealth;
	bCanMove = true;
}

void UCAttributeComponent::SetMove()
{
	bCanMove = true;
}

void UCAttributeComponent::SetStop()
{
	bCanMove = false;
}

void UCAttributeComponent::SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth)
{
	MaxHealth = InMaxHealth;

	CheckFalse(bResetCurrentHealth);
	CurrentHealth = MaxHealth;
}

void UCAttributeComponent::SetSpeed(ESpeedType InType)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeeds[(int32)InType];
}

void UCAttributeComponent::IncreaseHealth(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + InAmount, 0, MaxHealth);
}

void UCAttributeComponent::DecreaseHealth(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - InAmount, 0, MaxHealth);
}
