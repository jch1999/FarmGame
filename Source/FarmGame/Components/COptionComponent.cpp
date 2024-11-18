#include "COptionComponent.h"

UCOptionComponent::UCOptionComponent()
{
	MouseXSpeed = 90.0f;
	MouseYSpeed = 45.0f;

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
