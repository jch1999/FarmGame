#include "Components/CNutritionComponent.h"
#include "Global.h"

UCNutritionComponent::UCNutritionComponent()
{
}


void UCNutritionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCNutritionComponent::SetFamineState()
{
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Nutrition is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(ENutritionState::Famine);
}

void UCNutritionComponent::SetEnoughState()
{
	ChangeState(ENutritionState::Enough);
}

void UCNutritionComponent::SetOverState()
{
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Nutrition is over the safe rnage!", nullptr, FColor::Red, 0.8f);
	ChangeState(ENutritionState::Over);
}

void UCNutritionComponent::AddNutrition(float Amount)
{
	Super::AddValue(Amount);
	
	CheckState();
}

void UCNutritionComponent::ReduceNutrition(float Amount)
{
	Super::ReduceValue(Amount);

	CheckState();
}

void UCNutritionComponent::SetSafeRange(FVector2D InNewRange)
{
	Super::SetSafeRange(InNewRange);

	CheckState();
}


void UCNutritionComponent::AutoReduceValue()
{
	Super::AutoReduceValue();

	CheckState();
}

void UCNutritionComponent::CheckState()
{
	if (CurrentValue < SafeRange.X)
	{
		SetFamineState();
		return;
	}
	else if (CurrentValue > SafeRange.Y)
	{
		SetOverState();
		return;
	}

	SetEnoughState();
}

void UCNutritionComponent::ChangeState(ENutritionState state)
{
	CheckTrue(NutritionState == state);

	NutritionState = state;

	OnNutritionStateChanged.Broadcast(state);
}
