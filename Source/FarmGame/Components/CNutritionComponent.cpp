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
	NowNutrition += Amount;
	NowNutrition=FMath::Clamp(NowNutrition, 0, NutritionSafeRange.Y);

	CheckState();
}

void UCNutritionComponent::ReduceNutrition(float Amount)
{
	NowNutrition -= Amount;
	NowNutrition = FMath::Clamp(NowNutrition, 0, NutritionSafeRange.Y);

	CheckState();
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

void UCNutritionComponent::CheckState()
{
	if (NowNutrition < NutritionSafeRange.X)
	{
		SetFamineState();
		return;
	}
	else if (NowNutrition > NutritionSafeRange.Y)
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
