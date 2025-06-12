#include "Components/CGenericStateComponent.h"

UCGenericStateComponent::UCGenericStateComponent()
{
	MaxValue = 100.0f;
	if (SafeRange.Y == 0.0f)
	{
		SetSafeRange(FVector2D(0.0f, 100.0f));
	}
}


void UCGenericStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCGenericStateComponent::AddValue(float InAmount)
{
	float PrevValue = CurrentValue;
	CurrentValue += InAmount;
	CurrentValue = FMath::Clamp(CurrentValue, 0, MaxValue);

	OnStateValueChanged.Broadcast(PrevValue, CurrentValue, MaxValue);
	//UE_LOG(LogTemp, Warning, TEXT("AddValue : %.1f, CurrentValue : %.1f"), InAmount, CurrentValue);
}

void UCGenericStateComponent::ReduceValue(float ImAmount)
{
	float PrevValue = CurrentValue;
	CurrentValue -= ImAmount;
	CurrentValue = FMath::Clamp(CurrentValue, 0, MaxValue);

	OnStateValueChanged.Broadcast(PrevValue, CurrentValue, MaxValue);
	//UE_LOG(LogTemp, Warning, TEXT("ReduceValue : %.1f, CurrentValue : %.1f"), ImAmount, CurrentValue);
}

void UCGenericStateComponent::SetSafeRange(FVector2D InNewRange)
{
	SafeRange = InNewRange;
}

void UCGenericStateComponent::SetAutoReduceAmount(float InReduceAmount)
{
	AutoReduceAmount = InReduceAmount;
}

void UCGenericStateComponent::SetAutoReduceTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	if (GetWorld()->GetTimerManager().TimerExists(AutoReduceTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoReduceTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(AutoReduceTimer, this, &UCGenericStateComponent::AutoReduceValue, InLoopDelay, InbLoop, InFirstDelay);

}

void UCGenericStateComponent::AutoReduceValue()
{
	ReduceValue(AutoReduceAmount);
}
