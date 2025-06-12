#include "Components/CHealthComponent.h"
#include "Global.h"

UCHealthComponent::UCHealthComponent()
{
}


void UCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentValue = MaxValue;
}

void UCHealthComponent::SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth)
{
	MaxValue = InMaxHealth;

	CheckFalse(bResetCurrentHealth);
	float PrevHealth = CurrentValue;
	CurrentValue = MaxValue;

	OnStateValueChanged.Broadcast(PrevHealth, CurrentValue, MaxValue);
}

void UCHealthComponent::IncreaseHealth(float InAmount)
{
	Super::AddValue(InAmount);
}

void UCHealthComponent::DecreaseHealth(float InAmount)
{
	Super::ReduceValue(InAmount);
}
