#include "Components/CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	SneakSpeed = 200.0f;
	WalkSpeed = 400.0f;
	SprintSpeed = 600.0f;

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
