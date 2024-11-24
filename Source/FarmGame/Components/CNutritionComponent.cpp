#include "Components/CNutritionComponent.h"

UCNutritionComponent::UCNutritionComponent()
{
}


void UCNutritionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCNutritionComponent::AddNutrition(float Amount)
{
	NowNutrition += Amount;
	NowNutrition=FMath::Clamp(NowNutrition, 0, NutritionSafeRange.Y);

	if (IsUnder())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Nutrition is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
}

void UCNutritionComponent::ReduceNutrition(float Amount)
{
	NowNutrition -= Amount;
	NowNutrition = FMath::Clamp(NowNutrition, 0, NutritionSafeRange.Y);

	if (IsUnder())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), "Nutrition is under the safe rnage!", nullptr, FColor::Red, 0.8f);
	}
}

void UCNutritionComponent::SetSafeRange(FVector2D NewRange)
{
	NutritionSafeRange = NewRange;
}

void UCNutritionComponent::SetAutoReduceAmount(float InReduceAmount)
{
	AutoReduceAmount = InReduceAmount;
}

void UCNutritionComponent::SetAutoReduceTimer(float InFirstDelay, bool InbLoop, float InLoopDelay)
{
	GetWorld()->GetTimerManager().ClearTimer(NutiritionReduceTimer);

	GetWorld()->GetTimerManager().SetTimer(NutiritionReduceTimer, this, &UCNutritionComponent::AutoReduceNutirition, InLoopDelay, InbLoop, InFirstDelay);
}

void UCNutritionComponent::AutoReduceNutirition()
{
	ReduceNutrition(AutoReduceAmount);
}
