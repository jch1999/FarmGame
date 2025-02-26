#include "Components/COptionComponent.h"

UCOptionComponent::UCOptionComponent()
{
	MouseXSpeed = 45.0f;
	MouseYSpeed = 24.5f;

	ZoomSpeed = 1000.0f;
	ZoomRange = FVector2D(100.0f, 500.0f);
}

void UCOptionComponent::SetMouseXSpeed(float InSpeed)
{
	MouseXSpeed = InSpeed;
}

void UCOptionComponent::SetMouseYSpeed(float InSpeed)
{
	MouseYSpeed = InSpeed;
}
