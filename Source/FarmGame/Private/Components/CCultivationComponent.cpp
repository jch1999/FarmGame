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
	Super::AddValue(InAmount);
}

void UCCultivationComponent::ReduceCultivation(float InAmount)
{
	Super::ReduceValue(InAmount);
}
