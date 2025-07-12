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

void UCHealthComponent::SetHealthyState()
{
	ChangeState(EHealthState::Healthy);
}

void UCHealthComponent::SetSickState()
{
	ChangeState(EHealthState::Sick);
}

void UCHealthComponent::SetDeadlyState()
{
	ChangeState(EHealthState::Deadly);
}

void UCHealthComponent::SetDeadState()
{
	ChangeState(EHealthState::Dead);
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
	
	CheckState();
}

void UCHealthComponent::DecreaseHealth(float InAmount)
{
	Super::ReduceValue(InAmount);

	CheckState();
}

void UCHealthComponent::CheckState()
{
	if (CurrentValue <= 0.0f)
	{
		SetDeadState();
		return;
	}
	else if (CurrentValue < SafeRange.X)
	{
		SetDeadlyState();
		return;
	}
	else if (CurrentValue < SafeRange.Y)
	{
		SetSickState();
		return;
	}

	SetHealthyState();
}

void UCHealthComponent::ChangeState(EHealthState state)
{
	CheckTrue(HealthState == state);

	HealthState = state;

	OnHealthStateChanged.Broadcast(state);
}