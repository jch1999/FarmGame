#include "Components/CCultivationComponent.h"

UCCultivationComponent::UCCultivationComponent()
{
}


void UCCultivationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCCultivationComponent::AddCultivation(float InAmount)
{
	NowCultivation += InAmount;
	NowCultivation = FMath::Clamp(NowCultivation, CultivationRange.X, CultivationRange.Y);
}

void UCCultivationComponent::ReduceCultivation(float InAmount)
{
	NowCultivation -= InAmount;
	NowCultivation = FMath::Clamp(NowCultivation, CultivationRange.X, CultivationRange.Y);
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
