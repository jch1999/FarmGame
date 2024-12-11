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
	CurrentHealth = MaxHealth;

	OnHealthChanged.Broadcast();
}

void UCHealthComponent::IncreaseHealth(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + InAmount, 0, MaxHealth);

	OnHealthChanged.Broadcast();
}

void UCHealthComponent::DecreaseHealth(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - InAmount, 0, MaxHealth);

	OnHealthChanged.Broadcast();
}
