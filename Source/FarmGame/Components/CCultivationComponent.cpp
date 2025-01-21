#include "Components/CCultivationComponent.h"

UCCultivationComponent::UCCultivationComponent()
{
	MaxCultivation = 100.0f;
}


void UCCultivationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCCultivationComponent::AddCultivation(float InAmount)
{
	float PrevValue = CurrentCultivation;
	CurrentCultivation += InAmount;
	CurrentCultivation = FMath::Clamp(CurrentCultivation, CultivationRange.X, CultivationRange.Y);

	OnCultivationChanged.Broadcast(PrevValue, CurrentCultivation, GetMaxCultivation());
}

void UCCultivationComponent::ReduceCultivation(float InAmount)
{
	float PrevValue = CurrentCultivation;
	CurrentCultivation -= InAmount;
	CurrentCultivation = FMath::Clamp(CurrentCultivation, CultivationRange.X, CultivationRange.Y);

	OnCultivationChanged.Broadcast(PrevValue, CurrentCultivation, GetMaxCultivation());
}

void UCCultivationComponent::SetCultivationRange(FVector2D InNewRange)
{
	CultivationRange = InNewRange;
}

void UCCultivationComponent::SetAutoReduceAmount(float InReduceAmount)
{
	AutoReduceAmount = InReduceAmount;
}

void UCCultivationComponent::SetAutoReduceTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(CultivationReduceTimer);

	GetWorld()->GetTimerManager().SetTimer(CultivationReduceTimer, this, &UCCultivationComponent::AutoReduceCultivation, InLoopDelay, InbLoop, InFirstDelay);
}

void UCCultivationComponent::AutoReduceCultivation()
{
	ReduceCultivation(AutoReduceAmount);
}
