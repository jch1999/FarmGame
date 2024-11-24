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

	if (ISOver())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is over the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
	else if (IsUnder())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
}

void UCMoistureComponent::ReduceMoisture(float Amount)
{
	NowMoisture -= Amount;
	NowMoisture = FMath::Clamp(NowMoisture, 0, NowMoisture > MoistureSafeRange.Y ? NowMoisture : MoistureSafeRange.Y);
	if (IsUnder())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Moisture is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
}

void UCMoistureComponent::SetSafeRange(FVector2D NewRange)
{
	MoistureSafeRange = NewRange;
}

void UCMoistureComponent::SetAutoReduceAmount(float InReduceAmount)
{
	AutoReduceAmount = InReduceAmount;
}

void UCMoistureComponent::SetAutoReduceTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(MoistureReduceTimer);

	GetWorld()->GetTimerManager().SetTimer(MoistureReduceTimer, this, &UCMoistureComponent::AutoReduceMoisture, InLoopDelay, InbLoop, InFirstDelay);
}

void UCMoistureComponent::AutoReduceMoisture()
{
	ReduceMoisture(AutoReduceAmount);
}
