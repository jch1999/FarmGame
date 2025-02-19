#include "Components/CHealthComponent.h"
#include "Global.h"

UCHealthComponent::UCHealthComponent()
{
}


void UCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UCHealthComponent::SetMaxHealth(float InMaxHealth, bool bResetCurrentHealth)
{
	MaxHealth = InMaxHealth;

	CheckFalse(bResetCurrentHealth);
	float PrevHealth = CurrentHealth;
	CurrentHealth = MaxHealth;

	OnHealthChanged.Broadcast(PrevHealth,CurrentHealth,MaxHealth);
}

void UCHealthComponent::IncreaseHealth(float InAmount)
{
	float PrevHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + InAmount, 0, MaxHealth);

	OnHealthChanged.Broadcast(PrevHealth, CurrentHealth, MaxHealth);
}

void UCHealthComponent::DecreaseHealth(float InAmount)
{
	float PrevHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - InAmount, 0, MaxHealth);

	OnHealthChanged.Broadcast(PrevHealth, CurrentHealth, MaxHealth);
}
