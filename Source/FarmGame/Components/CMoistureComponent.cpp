#include "Components/CMoistureComponent.h"

UCMoistureComponent::UCMoistureComponent()
{
}


void UCMoistureComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCMoistureComponent::AddMoisture(float Amount)
{
	NowMoisture += Amount;
	if (NowMoisture < 0.0f)
	{
		NowMoisture = 0.0f;
	}

	if (ISOver())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is over the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
	else if (IsUnder())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
}

void UCMoistureComponent::SetSafeRange(FVector2D NewRange)
{
	MoistureSafeRange = NewRange;
}
